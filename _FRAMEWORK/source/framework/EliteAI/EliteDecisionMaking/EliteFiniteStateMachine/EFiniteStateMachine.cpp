//=== General Includes ===
#include "stdafx.h"
#include "EFiniteStateMachine.h"

Elite::FiniteStateMachine::FiniteStateMachine(Elite::FSMState* startState, Elite::Blackboard* pBlackboard)
    : m_pCurrentState(nullptr),
    m_pBlackboard(pBlackboard)
{
    ChangeState(startState);
}

Elite::FiniteStateMachine::~FiniteStateMachine()
{
    SAFE_DELETE(m_pBlackboard);
}

void Elite::FiniteStateMachine::AddTransition(Elite::FSMState* startState, Elite::FSMState* toState, Elite::FSMTransition* transition)
{
    auto it = m_Transitions.find(startState);
    if (it == m_Transitions.end())
    {
        m_Transitions[startState] = Transitions();
    }
   
    m_Transitions[startState].push_back(std::make_pair(transition, toState));
}

void Elite::FiniteStateMachine::Update(float deltaTime)
{
    // 4: Look if 1 or more transition exists for the current state that we are in
    //Tip: Check the transitions map for a TransitionState pair
    auto const it = m_Transitions.find(m_pCurrentState);

    // 5: if a TransitionState exists
    if (it != m_Transitions.end())
    {
        //6: Loop over all the TransitionState pairs 
        for (const auto& pair:it->second)
        {
            //7: If a ToTransition returns true => transition to the new corresponding state
            if (pair.first->ToTransition(m_pBlackboard))
            {
                ChangeState(pair.second);
                break;
            }
        }

    }

    //8: Update the current state (if one exists ;-))
    if (m_pCurrentState)
    {
        m_pCurrentState->Update(m_pBlackboard,deltaTime);
    }
    
}

Elite::Blackboard* Elite::FiniteStateMachine::GetBlackboard() const
{
    return m_pBlackboard;
}

void Elite::FiniteStateMachine::ChangeState(FSMState* newState)
{
    //1. If currently in a state => make sure the OnExit of that state gets called
    if (m_pCurrentState)
    {
        m_pCurrentState->OnExit(m_pBlackboard);
    }

    //2. Change the current state to the new state
    m_pCurrentState = newState;

    //3. Call the OnEnter of the new state
    if (m_pCurrentState)
    {
        m_pCurrentState->OnEnter(m_pBlackboard);
    }
        
}
