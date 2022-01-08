#include "stdafx.h"
#include "Integrator.h"

void Integrator::GenerateIntegrationField(Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>* pGraph, int endNodeIndex)
{
	vector<Elite::FlowFieldNode*> openList;
	vector<Elite::FlowFieldNode*> closedList;
	Elite::FlowFieldNode* currentNode = pGraph->GetNode(endNodeIndex);

	for (const auto node : pGraph->GetAllNodes())
	{
		node->SetIntegrationCost(255);
	}

	//Set the cost of the end node to 0 and add all its neighbours to the open list
	currentNode->SetIntegrationCost(0);

	for (auto currentConnection : pGraph->GetNodeConnections(currentNode))
	{
		openList.push_back(pGraph->GetNode(currentConnection->GetTo()));
	}
	closedList.push_back(currentNode);

	while (!openList.empty())
	{
		currentNode = openList[0];

		//Get the lowest cost of all the neighbouring nodes
		int lowestNeighbouringCost = INT_MAX;
		for (auto currentConnection : pGraph->GetNodeConnections(currentNode))
		{
			auto neighbourNode = pGraph->GetNode(currentConnection->GetTo());

			int neighbourCost = neighbourNode->GetIntegrationCost();
			if (lowestNeighbouringCost > neighbourCost)
			{
				lowestNeighbouringCost = neighbourCost;
			}

			//Add neighbour to the open list if it isn't on the closed list
			//This way we don't check the integration cost of nodes twice
			auto it = std::find(closedList.begin(), closedList.end(), neighbourNode);
			if (it == closedList.end())
				openList.push_back(pGraph->GetNode(currentConnection->GetTo()));
		}
		//Calculate integration cost
		int newCost = lowestNeighbouringCost + currentNode->GetCostFieldCost();
		currentNode->SetIntegrationCost(newCost);

		//Add the current node to the closed list
		std::_Erase_remove(openList, currentNode);
		closedList.push_back(currentNode);
	}
}
