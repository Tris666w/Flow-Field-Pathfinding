#pragma once
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"
using Graph2D = Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>*;

class FlowField
{
public:
	FlowField();
	~FlowField() = default;

	void GenerateIntegrationAndFlowField(Graph2D pGraph, int endNodeIndex);

	//Copy and move policy
	FlowField& operator=(const FlowField&) = delete;
	FlowField(const FlowField&) = delete;
	FlowField& operator=(FlowField&&) = delete;
	FlowField(FlowField&&) = delete;
private:
	vector<Elite::Vector2> m_DirectionVector;

	void GenerateIntegrationField(Graph2D pGraph, int endNodeIndex);
	void GenerateFlowField(Graph2D pGraph, int endNodeIndex);
};

