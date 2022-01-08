#pragma once
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"

using Graph2D = Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>*;

class Integrator final
{
public:
	Integrator() = default;
	~Integrator() = default;

	//Copy policy
	Integrator& operator=(const Integrator&) = delete;
	Integrator(const Integrator&) = delete;

	//move policy
	Integrator& operator=(Integrator&&) = delete;
	Integrator(Integrator&&) = delete;

	void GenerateIntegrationField(Graph2D pGraph, int endNodeIndex);
};

