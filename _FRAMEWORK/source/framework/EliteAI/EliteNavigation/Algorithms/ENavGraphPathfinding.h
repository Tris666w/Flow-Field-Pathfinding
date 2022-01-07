#pragma once
#include <vector>
#include <iostream>
#include "framework/EliteMath/EMath.h"
#include "framework\EliteAI\EliteGraphs\ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

namespace Elite
{
	class NavMeshPathfinding
	{
	public:
		static std::vector<Elite::Vector2> FindPath(Elite::Vector2 startPos, Elite::Vector2 endPos, Elite::NavGraph* pNavGraph, std::vector<Elite::Vector2>& debugNodePositions, std::vector<Elite::Portal>& debugPortals)
		{
			//Create the path to return
			std::vector<Elite::Vector2> finalPath{};

			//Get the start and endTriangle
			auto const startTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(startPos);
			auto const endTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(endPos);

			//We have valid start/end triangles and they are not the same
			if (startTriangle == nullptr || endTriangle == nullptr)
			{
				return finalPath;
			}
			else if (startTriangle == endTriangle)
			{
				finalPath.push_back(endPos);
				debugNodePositions = finalPath;
				return finalPath;
			}

			//=> Start looking for a path
			//Copy the graph
			const auto graphClone = pNavGraph->Clone();
			int index = pNavGraph->GetNrOfNodes();

			//Create extra node for the Start Node (Agent's position)
			auto* pStartNode = new NavGraphNode{ index++,-1,startPos };
			graphClone->AddNode(pStartNode);

			for (auto const lineIndex : startTriangle->metaData.IndexLines)
			{
				auto const nodeIdx = pNavGraph->GetNodeIdxFromLineIdx(lineIndex);
				if (nodeIdx != invalid_node_index)
				{
					auto const node = pNavGraph->GetNode(nodeIdx);
					float const cost = Distance(startPos, node->GetPosition());
					auto const newConnection = new GraphConnection2D{ pStartNode->GetIndex(),node->GetIndex(),cost };
					graphClone->AddConnection(newConnection);
				}
			}

			//Create extra node for the endNode
			auto* pEndNode = new NavGraphNode{ index++,-2,endPos };
			graphClone->AddNode(pEndNode);
			for (auto const lineIndex : endTriangle->metaData.IndexLines)
			{
				auto const nodeIdx = pNavGraph->GetNodeIdxFromLineIdx(lineIndex);
				if (nodeIdx != invalid_node_index)
				{
					auto const node = pNavGraph->GetNode(nodeIdx);
					float const cost = Distance(startPos, node->GetPosition());
					auto const newConnection = new GraphConnection2D{ pEndNode->GetIndex(),node->GetIndex(),cost };
					graphClone->AddConnection(newConnection);
				}
			}

			//Run A star on new graph
			auto aStar = AStar<NavGraphNode, GraphConnection2D>(graphClone.get(), HeuristicFunctions::Chebyshev);
			auto const tempFinal = aStar.FindPath(pStartNode, pEndNode);

			for (auto const pathNode : tempFinal)
			{
				finalPath.push_back(pathNode->GetPosition());
			}

			//OPTIONAL BUT ADVICED: Debug Visualisation
			debugNodePositions = finalPath;

			//Run optimiser on new graph, MAKE SURE the A star path is working properly before starting this section and uncommenting this!!!
			debugPortals = SSFA::FindPortals(tempFinal, pNavGraph->GetNavMeshPolygon());
			finalPath = SSFA::OptimizePortals(debugPortals);

			return finalPath;
		}

	};
}
