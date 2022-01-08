#include "stdafx.h"
#include "FlowField.h"


FlowField::FlowField()
{
	//All possible flow field directions
	m_DirectionVector.push_back({ 0,1 });//N
	m_DirectionVector.push_back({ 0.5,0.5 });//NE
	m_DirectionVector.push_back({ 1,0 }); //E
	m_DirectionVector.push_back({ 0.5,-0.5 }); //SE
	m_DirectionVector.push_back({ 0,-1 });//S
	m_DirectionVector.push_back({ -0.5,-0.5 });//SW
	m_DirectionVector.push_back({ -1,0 });//W
	m_DirectionVector.push_back({ -0.5,0.5 });//NW

	m_DirectionVector.resize(8);
}

void FlowField::GenerateIntegrationAndFlowField(Graph2D pGraph, int endNodeIndex)
{
	GenerateIntegrationField(pGraph, endNodeIndex);
	GenerateFlowField(pGraph, endNodeIndex);
}

void FlowField::GenerateIntegrationField(Graph2D pGraph, int endNodeIndex)
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

void FlowField::GenerateFlowField(Graph2D pGraph, int endNodeIndex)
{
}
