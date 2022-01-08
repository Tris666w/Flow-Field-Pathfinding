#pragma once
#include "framework/EliteAI/EliteGraphs/EGridGraph.h"

using Graph2D = Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>*;

class ISteeringBehavior;
class BlendedSteering;
class Wander;
class VelocityMatch;
class Cohesion;
class Seperation;
class CellSpace;
class FlowFieldSteering;
class SteeringAgent;

class FlowFieldFlock final
{
public:
	FlowFieldFlock(
		Graph2D flowField,
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
	Seperation* m_pSeparationBehavior = nullptr;
	VelocityMatch* m_pVelMatchBehavior = nullptr;
	BlendedSteering* m_pBlendedSteering = nullptr;
	FlowFieldSteering* m_pFlowFieldBehavior = nullptr;
};

