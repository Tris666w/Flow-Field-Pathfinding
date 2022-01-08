#pragma once
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
class FlowFieldFlock;

//COHESION - FLOCKING
//*******************
class Cohesion : public Seek
{
public:
	Cohesion(FlowFieldFlock* pFlock) :m_pFlock(pFlock) {};

	//Cohesion Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	FlowFieldFlock* m_pFlock = nullptr;
};


//SEPARATION - FLOCKING
//*********************
class Seperation : public Seek
{
public:
	Seperation(FlowFieldFlock* pFlock) :m_pFlock(pFlock) {};

	//Seperation Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	FlowFieldFlock* m_pFlock = nullptr;
};

//VELOCITY MATCH - FLOCKING
//************************
class VelocityMatch : public Seek
{
public:
	VelocityMatch(FlowFieldFlock* pFlock) :m_pFlock(pFlock) {};

	//VelocityMatch Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	FlowFieldFlock* m_pFlock = nullptr;
};