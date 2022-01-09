#pragma once
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"

using Graph2D = Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>*;

class FlowFieldSteering final : public ISteeringBehavior
{
public:
	explicit FlowFieldSteering(Graph2D flowFieldGraph);
	~FlowFieldSteering() = default;;

	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	Graph2D m_pFlowFieldGraph = nullptr;
};

//Evade OBSTACLE
//****************
class EvadeObstacle : public ISteeringBehavior
{
public:
	struct ObstacleInfo
	{
		Elite::Vector2 center;
		float distance;
	};


	explicit EvadeObstacle(std::vector<ObstacleBase*>* pObstacleVector);
	explicit EvadeObstacle(std::vector<ObstacleBase*>* pObstacleVector, float evadeRadius);
	~EvadeObstacle() = default;

	//Evade obstacle behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent)override;

	void SetEvadeRadius(float radius) { m_EvadeRadius = radius; }
private:
	float m_EvadeRadius = 25.f;
	std::vector<ObstacleBase*>* m_pObstacleVector = nullptr;
};
