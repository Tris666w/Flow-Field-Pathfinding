#include "stdafx.h"
#include "AgarioContactListener.h"
#include "AgarioData.h"
#include "AgarioFood.h"
#include "AgarioAgent.h"

AgarioContactListener::AgarioContactListener()
{
	PHYSICSWORLD->GetWorld()->SetContactListener(this);
}

void AgarioContactListener::BeginContact(b2Contact* contact)
{
	auto* bodyA = reinterpret_cast<Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>*>(contact->GetFixtureA()->GetUserData());
	auto* bodyB = reinterpret_cast<Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>*>(contact->GetFixtureB()->GetUserData());

	if (bodyA && bodyB)
	{
		Elite::RigidBodyUserData userDataA = bodyA->GetUserData();
		Elite::RigidBodyUserData userDataB = bodyB->GetUserData();

		if (AgarioObjectTypes(userDataA.Tag) == AgarioObjectTypes::Food)
		{
			if (AgarioObjectTypes(userDataB.Tag) == AgarioObjectTypes::Player)
			{
				OnCollisionFoodPlayer(reinterpret_cast<AgarioAgent*>(userDataB.pData), reinterpret_cast<AgarioFood*>(userDataA.pData));
			}
		}
		else if (AgarioObjectTypes(userDataA.Tag) == AgarioObjectTypes::Player)
		{
			if (AgarioObjectTypes(userDataB.Tag) == AgarioObjectTypes::Player)
			{
				OnCollisionPlayerPlayer(reinterpret_cast<AgarioAgent*>(userDataA.pData), reinterpret_cast<AgarioAgent*>(userDataB.pData));
			}
			else
			{
				OnCollisionFoodPlayer(reinterpret_cast<AgarioAgent*>(userDataA.pData), reinterpret_cast<AgarioFood*>(userDataB.pData));
			}
		}
	}

}

void AgarioContactListener::EndContact(b2Contact* contact)
{

}

void AgarioContactListener::OnCollisionPlayerPlayer(AgarioAgent* agentA, AgarioAgent* agentB)
{
	//Player-Player Collision
	//Check for enough size difference
	float agentDifference = abs(agentA->GetRadius() - agentB->GetRadius());
	if (agentDifference <= 1.0f)
	{
		return;
	}

	//Check for the biggest agent
	AgarioAgent* biggestAgent = agentA;
	AgarioAgent* smallestAgent = agentB;
	if (agentA->GetRadius() < agentB->GetRadius())
	{
		biggestAgent = agentB;
		smallestAgent = agentA;
	}

	//Remove Smallest Agent
	biggestAgent->MarkForUpgrade(smallestAgent->GetRadius() * m_FoodPerRadius);
	smallestAgent->MarkForDestroy();
}

void AgarioContactListener::OnCollisionFoodPlayer(AgarioAgent* agent, AgarioFood* food)
{
	//Player-Food Collision
	food->MarkForDestroy();
	agent->MarkForUpgrade();
}

