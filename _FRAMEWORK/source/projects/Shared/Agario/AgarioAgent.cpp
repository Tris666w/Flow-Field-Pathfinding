#include "stdafx.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "AgarioAgent.h"
#include "AgarioData.h"

using namespace Elite;
AgarioAgent::AgarioAgent(Elite::Vector2 pos, Color color)
	: SteeringAgent(2.0f)
{
	m_BodyColor = color;
	SetPosition(pos);
	SetMass(0.f);

	m_pRigidBody->SetUserData({ int(AgarioObjectTypes::Player), this });

	//Create the possible steering behaviors for the agent
	m_pWander = new Wander();
	m_pSeek = new Seek();
	m_pFlee = new Flee();
	m_pPursuit = new Pursuit();
}

AgarioAgent::AgarioAgent(Elite::Vector2 pos)
	: AgarioAgent(pos, Color{ 0.8f , 0.8f , 0.8f })
{
}

AgarioAgent::~AgarioAgent()
{
	SAFE_DELETE(m_DecisionMaking);
	SAFE_DELETE(m_pWander);
	SAFE_DELETE(m_pSeek);
	SAFE_DELETE(m_pFlee);
	SAFE_DELETE(m_pPursuit);
}

void AgarioAgent::Update(float dt)
{
	if (m_ToUpgrade > 0.0f)
	{
		OnUpgrade(m_ToUpgrade);
		m_ToUpgrade = 0.0f;
	}

	if(m_DecisionMaking)
		m_DecisionMaking->Update(dt);

	SteeringAgent::Update(dt);
}

void AgarioAgent::Render(float dt)
{
	SteeringAgent::Render(dt);
}

void AgarioAgent::MarkForUpgrade(float amountOfFood /* = 1.0f */)
{
	m_ToUpgrade = amountOfFood;
}

void AgarioAgent::MarkForDestroy()
{
	m_ToDestroy = true;
}

bool AgarioAgent::CanBeDestroyed()
{
	return m_ToDestroy;
}

void AgarioAgent::SetDecisionMaking(Elite::IDecisionMaking* decisionMakingStructure)
{
	m_DecisionMaking = decisionMakingStructure;
}

void AgarioAgent::SetToWander()
{
	SetSteeringBehavior(m_pWander);
}

void AgarioAgent::SetToSeek(Elite::Vector2 seekPos)
{
	m_pSeek->SetTarget(seekPos);
	SetSteeringBehavior(m_pSeek);
}

void AgarioAgent::SetToFlee(Elite::Vector2 fleePos)
{
	m_pFlee->SetTarget(fleePos);
	SetSteeringBehavior(m_pFlee);
}

void AgarioAgent::SetToPursuit(Elite::Vector2 pursuitPos)
{
	m_pPursuit->SetTarget(pursuitPos);
	SetSteeringBehavior(m_pPursuit);
}

void AgarioAgent::OnUpgrade(float amountOfFood)
{
	float prevMass = GetMass();
	m_Radius += amountOfFood;
	
	//Remove existing shapes
	m_pRigidBody->RemoveAllShapes();

	//Add a new shape
	Elite::EPhysicsCircleShape shape;
	shape.radius = m_Radius;
	m_pRigidBody->AddShape(&shape);
	m_pRigidBody->SetMass(0.f);
	SetMaxLinearSpeed(m_SpeedBase / sqrt(m_Radius));
}


