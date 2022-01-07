//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition(); //Calculate movement direction
	steering.LinearVelocity.Normalize(); // Normalize movement direction
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to max linear speed

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	}

	return steering;
}

//FLEE
//****
SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = pAgent->GetPosition() - m_Target.Position; //Calculate movement direction
	steering.LinearVelocity.Normalize(); // Normalize movement direction
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to max linear speed

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	}
	return steering;
}

//Arrive
//****
SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = { };

	steering.LinearVelocity = (m_Target).Position - pAgent->GetPosition(); //Desired velocity
	steering.LinearVelocity.Normalize(); //Normalize desired velocity
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to Max Speed

	float distance = Elite::DistanceSquared(m_Target.Position, pAgent->GetPosition());
	float slowDownDistance{ 100 };
	if (distance < slowDownDistance)
	{
		steering.LinearVelocity *= distance / slowDownDistance;
	}

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	}
	return steering;
}

SteeringOutput Face::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = { };
	steering.AngularVelocity = Elite::OrientationToVector(pAgent->GetOrientation()).AngleWith(m_Target.Position - pAgent->GetPosition());
	std::cout << steering.AngularVelocity << "\n";
	steering.AngularVelocity *= pAgent->GetMaxAngularSpeed();

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	}
	return steering;
}

SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	//1. Create a circle in front of the agent
	Elite::Vector2 center{};
	center.x = pAgent->GetPosition().x + (pAgent->GetDirection().x * m_OffsetDistance);
	center.y = pAgent->GetPosition().y + (pAgent->GetDirection().y * m_OffsetDistance);
	Elite::Circle const circle = { m_Radius, center };

	//2. Generate random angle
	float angle{ (rand() % 360 - 180) * float(M_PI / 180) };
	Elite::ClampRef(angle, -m_MaxAngleChange, m_MaxAngleChange);
	m_WanderAngle += angle;

	//3. Calculate new targetpoint
	Elite::Vector2 targetPoint{};
	targetPoint.x = circle.center.x + circle.radius * cos(m_WanderAngle);
	targetPoint.y = circle.center.y + circle.radius * sin(m_WanderAngle);

	//4. Calculate linear velocity
	steering.LinearVelocity = targetPoint - pAgent->GetPosition(); //Desired velocity
	steering.LinearVelocity.Normalize(); //Normalize desired velocity
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to Max Speed

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawPoint(targetPoint, 3, { 0,1,1 }, 0.3f);
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 1,0,0 }, 0.4f);
	}

	return steering;
}

SteeringOutput Pursuit::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	//1. Calculate Target Predicted position
	float const t = Elite::DistanceSquared(pAgent->GetPosition(), m_Target.Position) / (pAgent->GetMaxLinearSpeed() * pAgent->GetMaxLinearSpeed());
	Elite::Vector2 targetPos = m_Target.Position;
	targetPos += m_Target.LinearVelocity * t;

	//2. Calculate linear velocity
	steering.LinearVelocity = targetPos - pAgent->GetPosition(); //Desired velocity
	steering.LinearVelocity.Normalize(); //Normalize desired velocity
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to Max Speed

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawPoint(targetPos, 3, { 0,1,1 }, 0.3f);
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 1,0,0 }, 0.4f);
	}

	return steering;
}

SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	
	//1. Calculate Target Predicted position
	float const t = DistanceSquared(pAgent->GetPosition(), m_Target.Position) / (pAgent->GetMaxLinearSpeed() * pAgent->GetMaxLinearSpeed());
	Elite::Vector2 targetPos = m_Target.Position;
	targetPos += m_Target.LinearVelocity * t;

	if (Elite::Distance(pAgent->GetPosition(), m_Target.Position) > m_EvadeRadius)
	{
		steering.IsValid = false;
		return steering;
	}
	
	//2. Calcute linear velocity
	steering.LinearVelocity = pAgent->GetPosition() - targetPos; //Desired velocity
	steering.LinearVelocity.Normalize(); //Normalize desired velocity
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to Max Speed

	//DEBUG Rendering
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawPoint(targetPos, 3, { 0,0,1 }, 0.3f);
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 1,0,0 }, 0.4f);
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), m_EvadeRadius, { 1.f,1.f,1.f,1.f }, 0.3f);
	}

	return steering;
}
