#pragma once
#include "framework/EliteAI/EliteGraphs/EGridGraph.h"

using Graph2D = Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>*;

class Wander;
class Cohesion;
class CellSpace;
class Seperation;
class ObstacleBase;
class SteeringAgent;
class EvadeObstacle;
class VelocityMatch;
class BlendedSteering;
class PrioritySteering;
class ISteeringBehavior;
class FlowFieldSteering;

class FlowFieldFlock final
{
public:
	FlowFieldFlock(
		Graph2D flowField,
		std::vector<ObstacleBase*>* pObstacles,
		int nrCols,
		int nrRows,
		float cellSize,
		int flockSize,
		float agentSpeed);

	~FlowFieldFlock();

	void Update(float deltaT);
	const vector<SteeringAgent*>& GetNeighbors() const;
	int GetNrOfNeighbors() const;

	Elite::Vector2 GetAverageNeighborPos() const;
	Elite::Vector2 GetAverageNeighborVelocity() const;

	void ChangeAmountOfAgents(int newAmount);

	FlowFieldFlock(const FlowFieldFlock& other) = delete;
	FlowFieldFlock& operator=(const FlowFieldFlock& other) = delete;
	FlowFieldFlock(FlowFieldFlock&& other) = delete;
	FlowFieldFlock& operator=(FlowFieldFlock&& other) = delete;
private:
	int m_FlockSize = 0;
	float m_AgentSpeed = 16.f;
	float m_AgentEvadeRadius = 1.f;
	vector<SteeringAgent*> m_Agents;
	Elite::Vector2 m_WorldDimensions = { 150,150 };

	Graph2D m_pFlowFieldGraph = nullptr;

	//Spatial partitioning
	float m_NeighborhoodRadius = 15.f;
	CellSpace* m_pPartitionedSpace = nullptr;
	vector<Elite::Vector2> m_OldPositions = {};

	//----------------------------------
	//STEERING BEHAVIORS
	//----------------------------------
	Wander* m_pWanderBehavior = nullptr;
	Cohesion* m_pCohesionBehavior = nullptr;
	EvadeObstacle* m_pEvadeObstacle = nullptr;
	Seperation* m_pSeparationBehavior = nullptr;
	VelocityMatch* m_pVelMatchBehavior = nullptr;
	BlendedSteering* m_pBlendedSteering = nullptr;
	BlendedSteering* m_pFlockingBlendedSteering = nullptr;
	PrioritySteering* m_pPrioritySteering = nullptr;
	FlowFieldSteering* m_pFlowFieldBehavior = nullptr;
};

