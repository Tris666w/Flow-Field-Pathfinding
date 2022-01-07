/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
// Authors: Yosha Vandaele
/*=============================================================================*/
// EGridGraph.h: Derived graph type that automatically sets it up in a grid shape and sets up the connections
/*=============================================================================*/
#pragma once

#include "EIGraph.h"
#include "EGraphConnectionTypes.h"
#include "EGraphNodeTypes.h"

namespace Elite
{
	template<class T_NodeType, class T_ConnectionType>
	class GridGraph : public IGraph<T_NodeType, T_ConnectionType>
	{
	public:
		GridGraph(bool isDirectional);
		GridGraph(int columns, int rows, int cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f, float costDiagonal = 1.5);
		void InitializeGrid(int columns, int rows, int cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f, float costDiagonal = 1.5);

		using IGraph::GetNode;
		T_NodeType* GetNode(int col, int row) const { return m_Nodes[GetIndex(col, row)]; }
		const ConnectionList& GetConnections(const T_NodeType& node) const { return m_Connections[node.GetIndex()]; }
		const ConnectionList& GetConnections(int idx) const { return m_Connections[idx]; }

		int GetRows() const { return m_NrOfRows; }
		int GetColumns() const { return m_NrOfColumns; }

		bool IsWithinBounds(int col, int row) const;
		int GetIndex(int col, int row) const { return row * m_NrOfColumns + col; }

		// returns the column and row of the node in a Vector2
		using IGraph::GetNodePos;
		virtual Vector2 GetNodePos(T_NodeType* pNode) const override;

		// returns the actual world position of the node
		using IGraph::GetNodeWorldPos;
		Vector2 GetNodeWorldPos(int col, int row) const;
		Vector2 GetNodeWorldPos(int idx) const override;

		int GetNodeIdxAtWorldPos(const Elite::Vector2& pos) const override;

		void AddConnectionsToAdjacentCells(int col, int row);
		void AddConnectionsToAdjacentCells(int idx);
	private:
		
		int m_NrOfColumns;
		int m_NrOfRows;
		int m_CellSize;

		bool m_IsConnectedDiagonally;
		float m_DefaultCostStraight;
		float m_DefaultCostDiagonal;

		const vector<Vector2> m_StraightDirections = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
		const vector<Vector2> m_DiagonalDirections = { { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

		// graph creation helper functions
		void AddConnectionsInDirections(int idx, int col, int row, vector<Vector2> directions);

		float CalculateConnectionCost(int fromIdx, int toIdx) const;
	
		friend class GraphRenderer;
	};

	template<class T_NodeType, class T_ConnectionType>
	inline GridGraph<T_NodeType, T_ConnectionType>::GridGraph(bool isDirectional)
		: IGraph(isDirectional)
		, m_NrOfColumns(0)
		, m_NrOfRows(0)
		, m_CellSize(5)
		, m_IsConnectedDiagonally(true)
		, m_DefaultCostStraight(1.f)
		, m_DefaultCostDiagonal(1.5f)
	{
	}

	template<class T_NodeType, class T_ConnectionType>
	GridGraph<T_NodeType, T_ConnectionType>::GridGraph(
		int columns,
		int rows, 
		int cellSize, 
		bool isDirectionalGraph, 
		bool isConnectedDiagonally, 
		float costStraight /* = 1.f*/, 
		float costDiagonal /* = 1.5f */)
		: IGraph(isDirectionalGraph)
		, m_NrOfColumns(columns)
		, m_NrOfRows(rows)
		, m_CellSize(cellSize)
		, m_IsConnectedDiagonally(isConnectedDiagonally)
		, m_DefaultCostStraight(costStraight)
		, m_DefaultCostDiagonal(costDiagonal)
	{
		InitializeGrid(columns, rows, cellSize, isDirectionalGraph, isConnectedDiagonally, costStraight, costDiagonal);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void GridGraph<T_NodeType, T_ConnectionType>::InitializeGrid(
		int columns, 
		int rows, 
		int cellSize, 
		bool isDirectionalGraph,
		bool isConnectedDiagonally, 
		float costStraight /* = 1.f*/,
		float costDiagonal /* = 1.5f */)
	{
		m_IsDirectionalGraph = isDirectionalGraph;
		m_NrOfColumns = columns;
		m_NrOfRows = rows;
		m_CellSize = cellSize;
		m_IsConnectedDiagonally = isConnectedDiagonally;
		m_DefaultCostStraight = costStraight;
		m_DefaultCostDiagonal = costDiagonal;

		// Create all nodes
		for (auto r = 0; r < m_NrOfRows; ++r)
		{
			for (auto c = 0; c < m_NrOfColumns; ++c)
			{
				int idx = GetIndex(c, r);
				AddNode(new T_NodeType(idx));
			}
		}

		// Create connections in each valid direction on each node
		for (auto r = 0; r < m_NrOfRows; ++r)
		{
			for (auto c = 0; c < m_NrOfColumns; ++c)
			{
				AddConnectionsToAdjacentCells(c, r);
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	bool GridGraph<T_NodeType, T_ConnectionType>::IsWithinBounds(int col, int row) const
	{
		return (col >= 0 && col < m_NrOfColumns && row >= 0 && row < m_NrOfRows);
	}


	template<class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsToAdjacentCells(int col, int row)
	{
		int idx = GetIndex(col, row);

		// Add connections in all directions, taking into account the dimensions of the grid
		AddConnectionsInDirections(idx, col, row, m_StraightDirections);

		if (m_IsConnectedDiagonally)
		{
			AddConnectionsInDirections(idx, col, row, m_DiagonalDirections);
		}

		OnGraphModified(false, true);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsToAdjacentCells(int idx)
	{
		auto colRow = GetNodePos(idx);
		AddConnectionsToAdjacentCells((int)colRow.x, (int)colRow.y);
	}

	template<class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsInDirections(int idx, int col, int row, vector<Elite::Vector2> directions)
	{
		for (auto d : directions)
		{
			int neighborCol = col + (int)d.x;
			int neighborRow = row + (int)d.y;
			
			if (IsWithinBounds(neighborCol, neighborRow)) 
			{
				int neighborIdx = neighborRow * m_NrOfColumns + neighborCol;
				float connectionCost = CalculateConnectionCost(idx, neighborIdx);

				if (IsUniqueConnection(idx, neighborIdx) 
					&& connectionCost < 100000) //Extra check for different terrain types
					AddConnection(new GraphConnection(idx, neighborIdx, connectionCost));
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	inline float GridGraph<T_NodeType, T_ConnectionType>::CalculateConnectionCost(int fromIdx, int toIdx) const
	{
		float cost = m_DefaultCostStraight;

		Vector2 fromPos = GetNodePos(fromIdx);
		Vector2 toPos = GetNodePos(toIdx);

		if (int(fromPos.y) != int(toPos.y) &&
			int(fromPos.x) != int(toPos.x))
		{
			cost = m_DefaultCostDiagonal;
		}

		return cost;
	}

	template<>
	inline float GridGraph<GridTerrainNode, GraphConnection>::CalculateConnectionCost(int fromIdx, int toIdx) const
	{
		float cost = m_DefaultCostStraight;

		Vector2 fromPos = GetNodePos(fromIdx);
		Vector2 toPos = GetNodePos(toIdx);

		if (int(fromPos.y) != int(toPos.y) &&
			int(fromPos.x) != int(toPos.x))
		{
			cost = m_DefaultCostDiagonal;
		}

		cost *= (int(GetNode(fromIdx)->GetTerrainType()) + int(GetNode(toIdx)->GetTerrainType())) / 2.0f;

		return cost;
	}

	template<class T_NodeType, class T_ConnectionType>
	Elite::Vector2 GridGraph<T_NodeType, T_ConnectionType>::GetNodePos(T_NodeType* pNode) const
	{
		auto col = pNode->GetIndex() % m_NrOfColumns;
		auto row = pNode->GetIndex() / m_NrOfColumns;

		return Vector2{ float(col), float(row) };
	}

	template<class T_NodeType, class T_ConnectionType>
	Elite::Vector2 GridGraph<T_NodeType, T_ConnectionType>::GetNodeWorldPos(int col, int row) const
	{
		Vector2 cellCenterOffset = { m_CellSize / 2.f, m_CellSize / 2.f };
		return Vector2{ (float)col * m_CellSize, (float)row * m_CellSize } +cellCenterOffset;
	}

	template<class T_NodeType, class T_ConnectionType>
	Elite::Vector2 GridGraph<T_NodeType, T_ConnectionType>::GetNodeWorldPos(int idx) const
	{
		auto colRow = GetNodePos(idx);
		return GetNodeWorldPos((int)colRow.x, (int)colRow.y);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline int GridGraph<T_NodeType, T_ConnectionType>::GetNodeIdxAtWorldPos(const Elite::Vector2& pos) const
	{
		int idx = invalid_node_index;

		//Added extra check since  c = int(pos.x / m_CellSize); => doesnt work correcly when out of the lower bounds
		//TODO add grid start point
		if (pos.x < 0 || pos.y < 0)
		{
			return idx;
		}

		int r, c;

		c = int(pos.x / m_CellSize);
		r = int(pos.y / m_CellSize);

		if (!IsWithinBounds(c, r)) 
			return idx;

		return GetIndex(c, r);
	}
}