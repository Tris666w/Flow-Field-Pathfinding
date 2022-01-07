#pragma once
#include "../Steering/SteeringBehaviors.h"

class Flock;

//****************
//BLENDED STEERING
class BlendedSteering final: public ISteeringBehavior
{
public:
	struct WeightedBehavior
	{
		ISteeringBehavior* pBehavior = nullptr;
		float weight = 0.f;

		WeightedBehavior(ISteeringBehavior* pBehavior, float weight) :
			pBehavior(pBehavior),
			weight(weight)
		{};
	};

	BlendedSteering(vector<WeightedBehavior> weightedBehaviors);

	void AddBehaviour(WeightedBehavior weightedBehavior) { m_WeightedBehaviors.push_back(weightedBehavior); }
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	// returns a reference to the weighted behaviors, can be used to adjust weighting. Is not intended to alter the behaviors themselves.
	vector<WeightedBehavior>& GetWeightedBehaviorsRef() { return m_WeightedBehaviors; }

private:
	vector<WeightedBehavior> m_WeightedBehaviors = {};
};

//*****************
//PRIORITY STEERING
class PrioritySteering final: public ISteeringBehavior
{
public:
	PrioritySteering(vector<ISteeringBehavior*> priorityBehaviors) 
		:m_PriorityBehaviors(priorityBehaviors) 
	{}

	void AddBehaviour(ISteeringBehavior* pBehavior) { m_PriorityBehaviors.push_back(pBehavior); }
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	vector<ISteeringBehavior*> m_PriorityBehaviors = {};
};