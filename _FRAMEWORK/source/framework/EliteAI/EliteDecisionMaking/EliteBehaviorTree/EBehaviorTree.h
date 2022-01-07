/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EBehaviorTree.h: Implementation of a BehaviorTree and the components of a Behavior Tree
/*=============================================================================*/
#ifndef ELITE_BEHAVIOR_TREE
#define ELITE_BEHAVIOR_TREE

//--- Includes ---
#include "framework/EliteAI/EliteData/EBlackboard.h"

namespace Elite
{
	//-----------------------------------------------------------------
	// BEHAVIOR TREE HELPERS
	//-----------------------------------------------------------------
	enum BehaviorState
	{
		Failure,
		Success,
		Running
	};

	//-----------------------------------------------------------------
	// BEHAVIOR INTERFACES (BASE)
	//-----------------------------------------------------------------
	class IBehavior
	{
	public:
		IBehavior() = default;
		virtual ~IBehavior() = default;
		virtual BehaviorState Execute(Blackboard* pBlackBoard) = 0;

	protected:
		BehaviorState m_CurrentState = Failure;
	};

	//-----------------------------------------------------------------
	// BEHAVIOR TREE COMPOSITES (IBehavior)
	//-----------------------------------------------------------------
#pragma region COMPOSITES
	//--- COMPOSITE BASE ---
	class BehaviorComposite : public IBehavior
	{
	public:
		explicit BehaviorComposite(std::vector<IBehavior*> childrenBehaviors)
		{ m_ChildrenBehaviors = childrenBehaviors;	}
		virtual ~BehaviorComposite()
		{
			for (auto pb : m_ChildrenBehaviors)
				SAFE_DELETE(pb);
			m_ChildrenBehaviors.clear();
		}

		virtual BehaviorState Execute(Blackboard* pBlackBoard) override = 0;

	protected:
		std::vector<IBehavior*> m_ChildrenBehaviors = {};
	};

	//--- SELECTOR ---
	class BehaviorSelector : public BehaviorComposite
	{
	public:
		explicit BehaviorSelector(std::vector<IBehavior*> childrenBehaviors) :
			BehaviorComposite(childrenBehaviors) {}
		virtual ~BehaviorSelector() = default;

		virtual BehaviorState Execute(Blackboard* pBlackBoard) override;
	};

	//--- SEQUENCE ---
	class BehaviorSequence : public BehaviorComposite
	{
	public:
		explicit BehaviorSequence(std::vector<IBehavior*> childrenBehaviors) :
			BehaviorComposite(childrenBehaviors) {}
		virtual ~BehaviorSequence() = default;

		virtual BehaviorState Execute(Blackboard* pBlackBoard) override;
	};

	//--- PARTIAL SEQUENCE ---
	class BehaviorPartialSequence : public BehaviorSequence
	{
	public:
		explicit BehaviorPartialSequence(std::vector<IBehavior*> childrenBehaviors)
			: BehaviorSequence(childrenBehaviors) {}
		virtual ~BehaviorPartialSequence() = default;

		virtual BehaviorState Execute(Blackboard* pBlackBoard) override;

	private:
		unsigned int m_CurrentBehaviorIndex = 0;
	};
#pragma endregion

	//-----------------------------------------------------------------
	// BEHAVIOR TREE CONDITIONAL (IBehavior)
	//-----------------------------------------------------------------
	class BehaviorConditional : public IBehavior
	{
	public:
		explicit BehaviorConditional(std::function<bool(Blackboard*)> fp) : m_fpConditional(fp) {}
		virtual BehaviorState Execute(Blackboard* pBlackBoard) override;

	private:
		std::function<bool(Blackboard*)> m_fpConditional = nullptr;
	};

	//-----------------------------------------------------------------
	// BEHAVIOR TREE ACTION (IBehavior)
	//-----------------------------------------------------------------
	class BehaviorAction : public IBehavior
	{
	public:
		explicit BehaviorAction(std::function<BehaviorState(Blackboard*)> fp) : m_fpAction(fp) {}
		virtual BehaviorState Execute(Blackboard* pBlackBoard) override;

	private:
		std::function<BehaviorState(Blackboard*)> m_fpAction = nullptr;
	};

	//-----------------------------------------------------------------
	// BEHAVIOR TREE (BASE)
	//-----------------------------------------------------------------
	class BehaviorTree final : public Elite::IDecisionMaking
	{
	public:
		explicit BehaviorTree(Blackboard* pBlackBoard, IBehavior* pRootComposite)
			: m_pBlackBoard(pBlackBoard), m_pRootComposite(pRootComposite) {};
		~BehaviorTree()
		{
			SAFE_DELETE(m_pRootComposite);
			SAFE_DELETE(m_pBlackBoard); //Takes ownership of passed blackboard!
		};

		virtual void Update(float deltaTime) override
		{
			if (m_pRootComposite == nullptr)
			{
				m_CurrentState = Failure;
				return;
			}
				
			m_CurrentState = m_pRootComposite->Execute(m_pBlackBoard);
		}
		Blackboard* GetBlackboard() const
		{ return m_pBlackBoard;	}

	private:
		BehaviorState m_CurrentState = Failure;
		Blackboard* m_pBlackBoard = nullptr;
		IBehavior* m_pRootComposite = nullptr;
	};
}
#endif