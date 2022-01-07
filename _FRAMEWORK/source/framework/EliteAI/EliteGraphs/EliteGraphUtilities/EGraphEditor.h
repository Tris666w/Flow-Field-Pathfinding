#pragma once

#include "framework\EliteAI\EliteGraphs\EGraphNodeTypes.h"
#include "framework\EliteAI\EliteGraphs\EGraphConnectionTypes.h"
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"
#include "framework\EliteAI\EliteGraphs\EGraph2D.h"

namespace Elite 
{
	class GraphEditor final
	{
	public:
		GraphEditor() = default;
		~GraphEditor() = default;

		template <class T_NodeType, class T_ConnectionType>
		bool UpdateGraph(Graph2D<T_NodeType, T_ConnectionType>* pGraph);

		bool UpdateGraph(GridGraph<GridTerrainNode, GraphConnection>* pGraph);

		template <class T_NodeType, class T_ConnectionType>
		bool UpdateGraph(GridGraph<T_NodeType, T_ConnectionType>* pGraph);

	private:
		int m_SelectedNodeIdx = -1;
		int m_SelectedTerrainType = (int)TerrainType::Ground;

		Elite::Vector2 m_MousePos;
		bool m_IsLeftMouseBtnPressed = false;
	};
	
	template <class T_NodeType, class T_ConnectionType>
	inline bool GraphEditor::UpdateGraph(Graph2D<T_NodeType, T_ConnectionType>* pGraph)
	{
		bool hasGraphChanged = false;
		auto mouseMotionData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseMotion);

		if (INPUTMANAGER->IsMouseMoving())
			m_MousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld({ (float)mouseMotionData.X, (float)mouseMotionData.Y });

		if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eLeft))
			m_IsLeftMouseBtnPressed = false;

		// On left click:
		// - add connection
		// - select node
		// - add node
		if (INPUTMANAGER->IsMouseButtonDown(InputMouseButton::eLeft))
		{
			m_IsLeftMouseBtnPressed = true;
			int clickedIdx = pGraph->GetNodeIdxAtWorldPos(m_MousePos);

			if (m_SelectedNodeIdx != invalid_node_index)
			{
				if (clickedIdx != invalid_node_index && m_SelectedNodeIdx != clickedIdx)
				{
					if (pGraph->IsUniqueConnection(m_SelectedNodeIdx, clickedIdx))
						pGraph->AddConnection(new T_ConnectionType(m_SelectedNodeIdx, clickedIdx));
				}

				m_SelectedNodeIdx = invalid_node_index;
			}
			else if (clickedIdx != invalid_node_index)
			{
				m_SelectedNodeIdx = clickedIdx;
			}
			else
			{
				pGraph->AddNode(new T_NodeType(pGraph->GetNextFreeNodeIndex(), m_MousePos));
			}
		}

		// On right click:
		// - remove connection
		// - remove node
		if (INPUTMANAGER->IsMouseButtonDown(InputMouseButton::eRight))
		{
			auto clickedConnection = pGraph->GetConnectionAtPosition(m_MousePos);
			if (clickedConnection)
				pGraph->RemoveConnection(clickedConnection->GetFrom(), clickedConnection->GetTo());

			int clickedIdx = pGraph->GetNodeIdxAtWorldPos(m_MousePos);
			if (clickedIdx != invalid_node_index)
				pGraph->RemoveNode(clickedIdx);
		}

		// Update pNode, edge and debug drawing positions
		if (m_SelectedNodeIdx != invalid_node_index)
		{
			Elite::Vector2 nodePos = pGraph->GetNode(m_SelectedNodeIdx)->GetPosition();
			if (m_IsLeftMouseBtnPressed)
			{
				DEBUGRENDERER2D->DrawCircle(nodePos, pGraph->GetNodeRadius(pGraph->GetNode(m_SelectedNodeIdx)), { 1,1,1 }, -1);
				pGraph->GetNode(m_SelectedNodeIdx)->SetPosition(m_MousePos);
			}

			if (!m_IsLeftMouseBtnPressed)
				DEBUGRENDERER2D->DrawSegment(nodePos, m_MousePos, DEFAULT_CONNECTION_COLOR, -1);
		}

		return hasGraphChanged;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline bool GraphEditor::UpdateGraph(GridGraph<T_NodeType, T_ConnectionType>* pGraph)
	{
		bool hasGraphChanged = false;

		//Check if clicked on grid
		if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eLeft))
		{
			auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
			Vector2 mousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld({ (float)mouseData.X, (float)mouseData.Y });
			int idx = pGraph->GetNodeIdxAtWorldPos(mousePos);

			if (idx != invalid_node_index)
			{
				pGraph->AddConnectionsToAdjacentCells(idx);
				hasGraphChanged = true;
			}
		}
		if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eRight))
		{
			auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eRight);
			Vector2 mousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld({ (float)mouseData.X, (float)mouseData.Y });
			int idx = pGraph->GetNodeIdxAtWorldPos(mousePos);

			if (idx != invalid_node_index)
			{
				pGraph->RemoveConnectionsToAdjacentNodes(idx);
				hasGraphChanged = true;
			}
		}
		return hasGraphChanged;
	}
}


