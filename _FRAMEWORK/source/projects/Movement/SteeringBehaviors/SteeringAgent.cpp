#include "stdafx.h"
#include "SteeringAgent.h"
#include "Steering/SteeringBehaviors.h"

void SteeringAgent::Update(float dt)
{
	if(m_pSteeringBehavior)
	{
		auto output = m_pSteeringBehavior->CalculateSteering(dt, this);

		//Linear Movement
		//***************
		auto linVel = GetLinearVelocity();
		auto steeringForce = output.LinearVelocity - linVel;
		auto acceleration = steeringForce / GetMass();		

		if(m_RenderBehavior)
		{
			//DEBUGRENDERER2D->DrawDirection(GetPosition(), acceleration, acceleration.Magnitude(), { 0, 1, 1 ,0.5f }, 0.40f);
			//DEBUGRENDERER2D->DrawDirection(GetPosition(), linVel, linVel.Magnitude(), { 1, 0, 1 ,0.5f }, 0.40f);
		}
		SetLinearVelocity(linVel + (acceleration*dt));

		//Angular Movement
		//****************
		if(m_AutoOrient)
		{
			auto desiredOrientation = Elite::GetOrientationFromVelocity(GetLinearVelocity());
			SetRotation(desiredOrientation);
		}
		else
		{
			if (output.AngularVelocity > m_MaxAngularSpeed)
				output.AngularVelocity = m_MaxAngularSpeed;
			SetAngularVelocity(output.AngularVelocity);
		}
	}
}

void SteeringAgent::Render(float dt)
{
	//Use Default Agent Rendering
	BaseAgent::Render(dt);
}