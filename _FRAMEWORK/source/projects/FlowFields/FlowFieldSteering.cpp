#include "stdafx.h"
#include "FlowFieldSteering.h"

#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"

FlowFieldSteering::FlowFieldSteering(Graph2D flowFieldGraph)
	:m_pFlowFieldGraph(flowFieldGraph)
{
}

SteeringOutput FlowFieldSteering::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	//No valid flowField given
	if (!m_pFlowFieldGraph)
	{
		steering.IsValid = false;
		return steering;
	}

	//Get the cell the agent is in
	auto nodeIdx = m_pFlowFieldGraph->GetNodeIdxAtWorldPos(pAgent->GetPosition());

	if(nodeIdx < 0)
	{
		steering.IsValid = false;
		return steering;
	}


	//Get the move direction in the ce;;
	steering.LinearVelocity = m_pFlowFieldGraph->GetNode(nodeIdx)->GetDirection(); //Desired velocity
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to Max Speed

	return steering;
}
