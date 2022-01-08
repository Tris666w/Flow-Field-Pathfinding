#pragma once
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"
using Graph2D = Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>*;

class FlowField
{
public:
	FlowField() = default;
	~FlowField() = default;

	void GenerateIntegrationAndFlowField(Graph2D pGraph, int endNodeIndex);

	//Copy and move policy
	FlowField& operator=(const FlowField&) = delete;
	FlowField(const FlowField&) = delete;
	FlowField& operator=(FlowField&&) = delete;
	FlowField(FlowField&&) = delete;
private:
	void GenerateIntegrationField(Graph2D pGraph, int endNodeIndex);
	void GenerateFlowField(Graph2D pGraph, int endNodeIndex);
};

