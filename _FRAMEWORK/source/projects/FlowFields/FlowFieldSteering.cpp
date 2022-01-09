#include "stdafx.h"
#include "FlowFieldSteering.h"

#include "projects/Movement/SteeringBehaviors/ObstacleBase.h"
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

EvadeObstacle::EvadeObstacle(std::vector<ObstacleBase*>* pObstacleVector)
	:ISteeringBehavior(),m_pObstacleVector(pObstacleVector)
{
}

EvadeObstacle::EvadeObstacle(std::vector<ObstacleBase*>* pObstacleVector, float evadeRadius)
	: ISteeringBehavior(),m_EvadeRadius(evadeRadius), m_pObstacleVector(pObstacleVector)

{
}

SteeringOutput EvadeObstacle::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	std::vector<ObstacleInfo> pObstacleInfos{};

	//1.Get all obstacle in range and their distance
	for(auto const obstacleBase: *m_pObstacleVector)
	{
		float const distance = Distance(pAgent->GetPosition(), obstacleBase->GetCenter());
		if (distance <= m_EvadeRadius + obstacleBase->GetRadius())
		{
			pObstacleInfos.push_back({obstacleBase->GetCenter(),distance});
		}
	}

	//If there are no obstacles in range, the steering is invalid
	if (pObstacleInfos.empty())
	{
		steering.IsValid = false;
		return steering;
	}

	//Calculate the weight of every obstacle and their direction
	Elite::Vector2 sumVector{};
	for (auto const obstacleInfo : pObstacleInfos)
	{
	
		Elite::Vector2 dirVector = pAgent->GetPosition() - obstacleInfo.center;

		sumVector += (dirVector / obstacleInfo.distance);
	}
	sumVector /= static_cast<float>(pObstacleInfos.size());

	steering.LinearVelocity = sumVector; //Desired velocity
	steering.LinearVelocity.Normalize(); //Normalize desired velocity
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed(); //Rescale to Max Speed

	return steering;


}
