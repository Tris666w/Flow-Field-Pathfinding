#include "stdafx.h"
#include "ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

using namespace Elite;

Elite::NavGraph::NavGraph(const Polygon& contourMesh, float playerRadius = 1.0f) :
	Graph2D(false),
	m_pNavMeshPolygon(nullptr)
{
	//Create the navigation mesh (polygon of navigatable area= Contour - Static Shapes)
	m_pNavMeshPolygon = new Polygon(contourMesh); // Create copy on heap

	//Get all shapes from all static rigidbodies with NavigationCollider flag
	auto vShapes = PHYSICSWORLD->GetAllStaticShapesInWorld(PhysicsFlags::NavigationCollider);

	//Store all children
	for (auto shape : vShapes)
	{
		shape.ExpandShape(playerRadius);
		m_pNavMeshPolygon->AddChild(shape);
	}

	//Triangulate
	m_pNavMeshPolygon->Triangulate();

	//Create the actual graph (nodes & connections) from the navigation mesh
	CreateNavigationGraph();
}

Elite::NavGraph::~NavGraph()
{
	delete m_pNavMeshPolygon;
	m_pNavMeshPolygon = nullptr;
}

int Elite::NavGraph::GetNodeIdxFromLineIdx(int lineIdx) const
{
	auto nodeIt = std::find_if(m_Nodes.begin(), m_Nodes.end(), [lineIdx](const NavGraphNode* n) { return n->GetLineIndex() == lineIdx; });
	if (nodeIt != m_Nodes.end())
	{
		return (*nodeIt)->GetIndex();
	}

	return invalid_node_index;
}

Elite::Polygon* Elite::NavGraph::GetNavMeshPolygon() const
{
	return m_pNavMeshPolygon;
}

void Elite::NavGraph::CreateNavigationGraph()
{
	//1. Go over all the edges of the navigationmesh and create nodes
	int nodeIndex = 0;
	for (auto const pLine : m_pNavMeshPolygon->GetLines())
	{
		if (m_pNavMeshPolygon->GetTrianglesFromLineIndex(pLine->index).size() > 1) //Connected to another triangle
		{
			auto* pNewNode = new NavGraphNode{ nodeIndex++, pLine->index, CalculateMiddle(pLine->p1,pLine->p2) };
			AddNode(pNewNode);
		}
	}

	//2. Create connections now that every node is created
	for (auto const pTriangle : m_pNavMeshPolygon->GetTriangles())
	{
		std::vector<int> validNodeIndexVector{};
		for (auto const lineIndex : pTriangle->metaData.IndexLines)
		{
			for (auto const currentNode : m_Nodes)
			{
				if (currentNode->GetLineIndex() == lineIndex)
				{
					validNodeIndexVector.push_back(currentNode->GetIndex());
					break;
				}
			}
		}

		if (validNodeIndexVector.size() == 2)
		{
			auto const newNode = new GraphConnection2D{ validNodeIndexVector[0],validNodeIndexVector[1] };
			AddConnection(newNode);
		}
		else if (validNodeIndexVector.size() == 3)
		{
			auto const newNode1 = new GraphConnection2D{ validNodeIndexVector[0],validNodeIndexVector[1] };
			auto const newNode2 = new GraphConnection2D{ validNodeIndexVector[1],validNodeIndexVector[2] };
			auto const newNode3 = new GraphConnection2D{ validNodeIndexVector[2],validNodeIndexVector[0] };

			AddConnection(newNode1);
			AddConnection(newNode2);
			AddConnection(newNode3);
		}

	}
	//3. Set the connections cost to the actual distance
	SetConnectionCostsToDistance();
}

