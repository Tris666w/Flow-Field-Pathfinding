/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "../SteeringHelpers.h"
class SteeringAgent;
class ObstacleBase;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) = 0;

	//Seek Functions
	void SetTarget(const TargetData& target) { m_Target = target; }

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{
		return static_cast<T*>(this);
	}

protected:
	TargetData m_Target;
};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

/////////////////////////
//FLEE
//****
class Flee : public Seek
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	//Flee Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

/////////////////////////
//Arrive
//****
class Arrive : public Seek
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	//Arrive Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

/////////////////////////
//Face
//****
class Face : public Seek
{
public:
	Face() = default;
	virtual ~Face() = default;

	//Face Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

/////////////////////////
//Wander
//****
class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetWanderOffset(float offset) { m_OffsetDistance = offset; }
	void SetWanderRadius(float radius) { m_Radius = radius; }
	void SetMaxAngleChange(float rad) { m_MaxAngleChange = rad; }
protected:
	float m_OffsetDistance = 4.f; //Offset (agent direction)
	float m_Radius = 3.f; //WanderRadius
	float m_MaxAngleChange = Elite::ToRadians(15); // Max wanderangle change per frame
	float m_WanderAngle = 0.f; //Internal
};

//Pursuit
//****
class Pursuit : public Seek
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;

	//Pursuit Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

//Evade
//****
class Evade : public Seek
{
public:
	Evade() = default;
	virtual ~Evade() = default;

	//Evade Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
	
	void SetEvadeRadius(float radius) { m_EvadeRadius = radius * radius; }

protected:
	float m_EvadeRadius = 40.f;
};
#endif
