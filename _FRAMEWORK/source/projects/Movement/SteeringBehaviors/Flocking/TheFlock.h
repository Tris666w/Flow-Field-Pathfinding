#pragma once
#include "../SteeringHelpers.h"
#include "FlockingSteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/SpacePartitioning/SpacePartitioning.h"


class ISteeringBehavior;
class SteeringAgent;
class BlendedSteering;
class PrioritySteering;


#define USE_SPACE_PARTITIONING

class Flock
{
public:
	Flock(
		int flockSize = 50,
		float worldSize = 100.f,
		SteeringAgent* pAgentToEvade = nullptr,
		bool trimWorld = false);

	~Flock();

	void Update(float deltaT);
	void UpdateAndRenderUI();
	void Render(float deltaT);


	const vector<SteeringAgent*>& GetNeighbors() const;
	int GetNrOfNeighbors() const;

	void RegisterNeighbors(SteeringAgent* pAgent);


	Elite::Vector2 GetAverageNeighborPos() const;
	Elite::Vector2 GetAverageNeighborVelocity() const;

	void SetSeekTarget(TargetData target) const;
	void SetWorldTrimSize(float size) { m_WorldSize = size; }

private:
	//Datamembers
	int m_FlockSize = 0;
	vector<SteeringAgent*> m_Agents;

	bool m_UsesPartitioning = true;;
	CellSpace* m_pPartitionedSpace = nullptr;
	int m_NrPartitionCols = 25;
	int m_NrPartitionRows = 25;

	vector<Elite::Vector2> m_OldPositions = {};

	vector<SteeringAgent*> m_Neighbors;


	bool m_TrimWorld = false;
	float m_WorldSize = 0.f;

	float m_NeighborhoodRadius = 15.f;
	int m_NrOfNeighbors = 0;

	//Debug render member variables
	bool m_DebugRenderNeighborhood = false;
	bool m_DebugRenderSteering = false;
	bool m_DebugPartitionGrid = false;

	//Evade target
	SteeringAgent* m_pAgentToEvade = nullptr;

	//Steering Behaviors
	Seperation* m_pSeparationBehavior = nullptr;
	Cohesion* m_pCohesionBehavior = nullptr;
	VelocityMatch* m_pVelMatchBehavior = nullptr;
	Seek* m_pSeekBehavior = nullptr;
	Wander* m_pWanderBehavior = nullptr;
	Evade* m_pEvadeBehavior = nullptr;

	BlendedSteering* m_pBlendedSteering = nullptr;
	PrioritySteering* m_pPrioritySteering = nullptr;

	float* GetWeight(ISteeringBehavior* pBehaviour) const;
	void UpdateEvadeTarget() const;

private:
	Flock(const Flock& other);
	Flock& operator=(const Flock& other);
};