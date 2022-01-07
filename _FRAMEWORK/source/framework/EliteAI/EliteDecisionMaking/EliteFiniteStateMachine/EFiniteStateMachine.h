/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
// Authors: Andries Geens
// http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter12_A_Reusable_Light-Weight_Finite-State_Machine.pdf
/*=============================================================================*/
// EStateMachine.h: Other implementation of a FSM
/*=============================================================================*/
#ifndef ELITE_FINITE_STATE_MACHINE
#define ELITE_FINITE_STATE_MACHINE

namespace Elite
{
	class FSMState
	{
	public:
		FSMState(){}
		virtual ~FSMState() = default;

		virtual void OnEnter(Blackboard* pBlackboard) {};
		virtual void OnExit(Blackboard* pBlackboard) {};
		virtual void Update(Blackboard* pBlackboard, float deltaTime) {};

	};

	class FSMTransition
	{
	public:
		FSMTransition() = default;
		virtual ~FSMTransition() = default;
		virtual bool ToTransition(Blackboard* pBlackboard) const = 0;
	};

	class FiniteStateMachine final : public Elite::IDecisionMaking
	{
	public:
		FiniteStateMachine(FSMState* startState, Blackboard* pBlackboard);
		virtual ~FiniteStateMachine();
		
		void AddTransition(FSMState* startState, FSMState* toState, FSMTransition* transition);
		virtual void Update(float deltaTime) override;
		Elite::Blackboard* GetBlackboard() const;

	private:
		void ChangeState(FSMState* newState);
	private:
		typedef std::pair<FSMTransition*, FSMState*> TransitionStatePair;
		typedef std::vector<TransitionStatePair> Transitions;

		std::map<FSMState*, Transitions> m_Transitions; //Key is the state, value are all the transitions for that current state 
		FSMState* m_pCurrentState;
		Blackboard* m_pBlackboard = nullptr; // takes ownership of the blackboard
	};

}
#endif