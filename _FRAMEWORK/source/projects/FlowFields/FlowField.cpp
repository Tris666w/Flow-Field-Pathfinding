#include "stdafx.h"
#include "FlowField.h"

void FlowField::GenerateIntegrationAndFlowField(Graph2D pGraph, int endNodeIndex)
{
	GenerateIntegrationField(pGraph, endNodeIndex);
	GenerateFlowField(pGraph, endNodeIndex);
}

void FlowField::GenerateIntegrationField(Graph2D pGraph, int endNodeIndex)
{
	if (endNodeIndex == -1)
	{
		peach::Logger::LogError("Selected node is outside the map, please respect the world boundaries");
		return;
	}
	vector<Elite::FlowFieldNode*> openList;
	vector<Elite::FlowFieldNode*> closedList;
	Elite::FlowFieldNode* currentNode = pGraph->GetNode(endNodeIndex);

	for (const auto node : pGraph->GetAllNodes())
	{
		node->SetIntegrationCost(255);
	}

	//Set the cost of the end node to 0 and add all its neighbors to the open list
	currentNode->SetIntegrationCost(0);

	for (auto currentConnection : pGraph->GetNodeConnections(currentNode))
	{
		openList.push_back(pGraph->GetNode(currentConnection->GetTo()));
	}
	closedList.push_back(currentNode);

	while (!openList.empty())
	{
		currentNode = openList[0];

		if (currentNode->GetCostFieldCost() == 255)
		{
			currentNode->SetIntegrationCost(255);
			std::_Erase_remove(openList, currentNode);
			closedList.push_back(currentNode);
			continue;
		}

		//Get the lowest cost of all the neighboring nodes
		int lowestNeighboringCost = INT_MAX;
		for (auto currentConnection : pGraph->GetNodeConnections(currentNode))
		{
			auto neighborNode = pGraph->GetNode(currentConnection->GetTo());

			int neighborCost = neighborNode->GetIntegrationCost();
			if (lowestNeighboringCost > neighborCost)
			{
				lowestNeighboringCost = neighborCost;
			}

			//Add neighbor to the open list if it isn't on the closed list
			//This way we don't check the integration cost of nodes twice
			auto it = std::find(closedList.begin(), closedList.end(), neighborNode);
			if (it == closedList.end())
				openList.push_back(pGraph->GetNode(currentConnection->GetTo()));
		}
		//Calculate integration cost
		int newCost = lowestNeighboringCost + currentNode->GetCostFieldCost();
		currentNode->SetIntegrationCost(newCost);

		//Add the current node to the closed list
		std::_Erase_remove(openList, currentNode);
		closedList.push_back(currentNode);
	}
}

void FlowField::GenerateFlowField(Graph2D pGraph, int endNodeIndex)
{
	for (const auto node : pGraph->GetAllNodes())
	{
		node->SetDirection({ 0,0 });
	}

	for (auto pNode : pGraph->GetAllNodes())
	{
		Elite::Vector2 dir = { 0,0 };
		if (pNode->GetIntegrationCost() >= 255)
		{
			pNode->SetDirection(dir);
			continue;
		}
		//Get the neighbor with the lowest integration cost
		int lowestNeighboringCost = INT_MAX;
		Elite::FlowFieldNode* pClosestNeighbor = nullptr;

		for (auto currentConnection : pGraph->GetNodeConnections(pNode))
		{
			auto neighborNode = pGraph->GetNode(currentConnection->GetTo());

			int neighborCost = neighborNode->GetIntegrationCost();
			if (lowestNeighboringCost > neighborCost)
			{
				lowestNeighboringCost = neighborCost;
				pClosestNeighbor = neighborNode;
			}
		}
		if (pClosestNeighbor != nullptr)
		{
			//Calculate direction
			dir = pGraph->GetNodeWorldPos(pClosestNeighbor->GetIndex()) - pGraph->GetNodeWorldPos(pNode->GetIndex());
			dir.Normalize();
			pNode->SetDirection(dir);
		}
	}
}
