#pragma once
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
class Flock;

//COHESION - FLOCKING
//*******************
class Cohesion : public Seek
{
public:
	Cohesion(Flock* pFlock) :m_pFlock(pFlock) {};

	//Cohesion Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	Flock* m_pFlock = nullptr;
};


//SEPARATION - FLOCKING
//*********************
class Seperation : public Seek
{
public:
	Seperation(Flock* pFlock) :m_pFlock(pFlock) {};

	//Seperation Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	Flock* m_pFlock = nullptr;
};

//VELOCITY MATCH - FLOCKING
//************************
class VelocityMatch : public Seek
{
public:
	VelocityMatch(Flock* pFlock) :m_pFlock(pFlock) {};

	//VelocityMatch Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	Flock* m_pFlock = nullptr;
};