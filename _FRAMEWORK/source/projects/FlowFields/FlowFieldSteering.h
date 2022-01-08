#pragma once
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"
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

