//=== General Includes ===
#include "stdafx.h"
using namespace Elite;

//-----------------------------------------------------------------
// BEHAVIOR TREE COMPOSITES (IBehavior)
//-----------------------------------------------------------------
#pragma region COMPOSITES
//SELECTOR
BehaviorState BehaviorSelector::Execute(Blackboard* pBlackBoard)
{
	for (auto child : m_ChildrenBehaviors)
	{
		m_CurrentState = child->Execute(pBlackBoard);
		switch (m_CurrentState)
		{
		case Failure:
			continue; break;
		case Success:
			return m_CurrentState; break;
		case Running:
			return m_CurrentState; break;
		default:
			continue; break;
		}
	}
	return m_CurrentState = Failure;
}
//SEQUENCE
BehaviorState BehaviorSequence::Execute(Blackboard* pBlackBoard)
{
	for (auto child : m_ChildrenBehaviors)
	{
		m_CurrentState = child->Execute(pBlackBoard);
		switch (m_CurrentState)
		{
		case Failure:
			return m_CurrentState; break;
		case Success:
			continue; break;
		case Running:
			return m_CurrentState; break;
		default:
			m_CurrentState = Success;
			return m_CurrentState; break;
		}
	}
	return m_CurrentState = Success;
}
//PARTIAL SEQUENCE
BehaviorState BehaviorPartialSequence::Execute(Blackboard* pBlackBoard)
{
	while (m_CurrentBehaviorIndex < m_ChildrenBehaviors.size())
	{
		m_CurrentState = m_ChildrenBehaviors[m_CurrentBehaviorIndex]->Execute(pBlackBoard);
		switch (m_CurrentState)
		{
		case Failure:
			m_CurrentBehaviorIndex = 0;
			return m_CurrentState; break;
		case Success:
			++m_CurrentBehaviorIndex;
			return m_CurrentState = Running; break;
		case Running:
			return m_CurrentState; break;
		}
	}

	m_CurrentBehaviorIndex = 0;
	return m_CurrentState = Success;
}
#pragma endregion
//-----------------------------------------------------------------
// BEHAVIOR TREE CONDITIONAL (IBehavior)
//-----------------------------------------------------------------
BehaviorState BehaviorConditional::Execute(Blackboard* pBlackBoard)
{
	if (m_fpConditional == nullptr)
		return Failure;

	switch (m_fpConditional(pBlackBoard))
	{
	case true:
		return m_CurrentState = Success;
	case false:
		return m_CurrentState = Failure;
	}
	return m_CurrentState = Failure;
}
//-----------------------------------------------------------------
// BEHAVIOR TREE ACTION (IBehavior)
//-----------------------------------------------------------------
BehaviorState BehaviorAction::Execute(Blackboard* pBlackBoard)
{
	if (m_fpAction == nullptr)
		return Failure;

	return m_CurrentState = m_fpAction(pBlackBoard);
}