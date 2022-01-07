#include "stdafx.h"
#include "FlockingSteeringBehaviors.h"
#include "TheFlock.h"
#include "../SteeringAgent.h"
#include "../SteeringHelpers.h"

//*******************
//COHESION (FLOCKING)
SteeringOutput Cohesion::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	if (m_pFlock->GetNrOfNeighbors() <= 0)
	{
		SteeringOutput steering;
		steering.IsValid = false;
		return steering;
	}

	SteeringOutput steering = { };

	steering.LinearVelocity = m_pFlock->GetAverageNeighborPos() - pAgent->GetPosition(); //Desired velocity
	steering.LinearVelocity.Normalize(); //Normalize desired velocity
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to Max Speed

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	}
	return steering;
}


//*********************
//SEPARATION (FLOCKING)
SteeringOutput Seperation::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	if (m_pFlock->GetNrOfNeighbors() == 0)
	{
		SteeringOutput steering{};
		steering.IsValid = false;
		return steering;
	}
	
	Elite::Vector2 sumVector{};
	for (int i = 0; i < m_pFlock->GetNrOfNeighbors(); i++)
	{
		float const distanceToNeighbor= Distance(pAgent->GetPosition(), m_pFlock->GetNeighbors()[i]->GetPosition());
		Elite::Vector2 neighborVector = pAgent->GetPosition() - m_pFlock->GetNeighbors()[i]->GetPosition();

		sumVector += (neighborVector / distanceToNeighbor);
	}
	sumVector /= static_cast<float>(m_pFlock->GetNrOfNeighbors());
	
	SteeringOutput steering = { };

	steering.LinearVelocity = sumVector; //Desired velocity
	steering.LinearVelocity.Normalize(); //Normalize desired velocity
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to Max Speed

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	}
	return steering;
}


//*************************
//VELOCITY MATCH (FLOCKING)
SteeringOutput VelocityMatch::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	if (m_pFlock->GetNrOfNeighbors() == 0)
	{
		SteeringOutput steering;
		steering.IsValid = false;
		return steering;
	}
	
	SteeringOutput steering = { };

	steering.LinearVelocity = m_pFlock->GetAverageNeighborVelocity(); //Desired velocity
	steering.LinearVelocity.Normalize(); //Normalize desired velocity
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to Max Speed

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	}
	return steering;
}
