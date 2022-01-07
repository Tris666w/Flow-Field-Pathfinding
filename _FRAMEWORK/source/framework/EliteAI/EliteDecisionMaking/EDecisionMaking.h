/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
// Authors: Andries Geens
/*=============================================================================*/
// EDecisionMaking.h: General Decision making header that includes all Decision making structures and blackboard data
/*=============================================================================*/
#ifndef ELITE_DECISION_MAKING
#define ELITE_DECISION_MAKING

/* --- Data --- */
//General Interface
namespace Elite
{
	class IDecisionMaking
	{
	public:
		IDecisionMaking() = default;
		virtual ~IDecisionMaking() = default;

		virtual void Update(float deltaT) = 0;

	};
}

/* --- Data --- */
//Blackboard
#include "framework/EliteAI/EliteData/EBlackboard.h"

/* --- Decision Making Structures --- */
//FSM & BT
#include "framework/EliteAI/EliteDecisionMaking/EliteFiniteStateMachine/EFiniteStateMachine.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteBehaviorTree/EBehaviorTree.h"


#endif



