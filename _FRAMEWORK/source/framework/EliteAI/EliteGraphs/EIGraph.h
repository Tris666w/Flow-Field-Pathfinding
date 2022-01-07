/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
// Authors: Yosha Vandaele
/*=============================================================================*/
// EIGraph.h: Abstract graph implementation, heavily based on the implementation from "Programming Game AI By Example" by Mat Buckland
/*=============================================================================*/

#pragma once

#include "EGraphNodeTypes.h"
#include "EGraphConnectionTypes.h"
#include <memory>

namespace Elite
{
	template <class T_NodeType, class T_ConnectionType>
	class IGraph
	{
	public:
		// shorthand typedefs
		using NodeVector = std::vector<T_NodeType*>;
		using ConnectionList = std::list<T_ConnectionType*>; // TODO: function definition doesn't recognize this?
		using ConnectionListVector = std::vector<ConnectionList>;

	public:
		IGraph(bool isDirectionalGraph);
		IGraph(const IGraph& other);
		virtual ~IGraph();

		// Basic graph functionality
		// -------------------------
		T_NodeType* GetNode(int idx) const;
		bool IsNodeValid(int idx) const;
		const NodeVector& GetAllNodes() const { return m_Nodes; }
		NodeVector GetAllActiveNodes() const;

		T_ConnectionType* GetConnection(int from, int to) const;
		const ConnectionListVector& GetAllConnections() const { return m_Connections; }
		const ConnectionList& GetNodeConnections(int idx) const;
		const ConnectionList& GetNodeConnections(T_NodeType* pNode) const { return GetNodeConnections(pNode->GetIndex()); }

		int GetNextFreeNodeIndex() const { return m_NextNodeIndex; }
		int AddNode(T_NodeType* pNode);
		void RemoveNode(int node);

		void AddConnection(T_ConnectionType* pConnection);
		void RemoveConnection(int from, int to);
		void RemoveConnection(T_ConnectionType* pConnection);

		// Removes all connections to this pNode
		void RemoveConnectionsToAdjacentNodes(int idx);

		void SetConnectionCost(int from, int to, float cost);

		int GetNrOfNodes() const { return m_Nodes.size(); }
		int GetNrOfActiveNodes() const; // TODO: add comment
		int GetNrOfConnections() const;
		bool IsDirectionalGraph() const { return m_IsDirectionalGraph; }
		bool IsEmpty() const { return m_Nodes.empty(); }
		bool IsUniqueConnection(int from, int to) const;

		void Clear();
		void RemoveConnections();

		// Visualization
		// -------------
		float GetNodeRadius(T_NodeType* pNode) const;

		// (Pure) virtuals to be override by specific graph types
		// ----------------------------------------------------
		virtual Vector2 GetNodePos(T_NodeType* pNode) const = 0;
		Vector2 GetNodePos(int idx) const { return GetNodePos(GetNode(idx)); }

		// Provide the opportunity for derived classes to differentiate the conceptual position from the world position
		// Example: A grid position might consist of rows and columns, while the world position is expressed as a (x,y) coordinate
		virtual Vector2 GetNodeWorldPos(int idx) const { return GetNodePos(idx); }
		Vector2 GetNodeWorldPos(T_NodeType* pNode) const { return GetNodeWorldPos(pNode->GetIndex()); }

		virtual int GetNodeIdxAtWorldPos(const Elite::Vector2& pos) const = 0;
		T_NodeType* GetNodeAtWorldPos(const Elite::Vector2& pos) const { return IsNodeValid(GetNodeIdxAtWorldPos(pos)) ? GetNode(GetNodeIdxAtWorldPos(pos)) : nullptr; }

		// Allow derived classes to implement a cloning function that returns a base class pointer
		virtual shared_ptr<IGraph<T_NodeType, T_ConnectionType>> Clone() const { return nullptr; };

	protected:
		// A vector of adjacency pConnection lists, mapped to the indices of the nodes
		// m_Edges[0] returns the list of connections of the pNode with index 0
		ConnectionListVector m_Connections;
		NodeVector m_Nodes;

		bool m_IsDirectionalGraph;


		// Called whenever the graph is modified, to be overriden by derived classes
		virtual void OnGraphModified(bool nrOfNodesChanged, bool nrOfConnectionsChanged) {}

	private:
		int m_NextNodeIndex;

		// private functions
		void CullInvalidEdges();
	};

	template<class T_NodeType, class T_ConnectionType>
	inline IGraph<T_NodeType, T_ConnectionType>::IGraph(bool isDirectionalGraph)
		: m_NextNodeIndex(0)
		, m_IsDirectionalGraph(isDirectionalGraph)
	{
	}

	template<class T_NodeType, class T_ConnectionType>
	inline IGraph<T_NodeType, T_ConnectionType>::IGraph(const IGraph& other)
	{
		for (auto& n : m_Nodes)
			SAFE_DELETE(n);

		for (auto& connectionList : m_Connections)
		{
			for (auto& connection : connectionList)
				SAFE_DELETE(connection);
		};

		for (auto n : other.m_Nodes)
			m_Nodes.push_back(new T_NodeType(*n));

		for (auto cList : other.m_Connections)
		{
			ConnectionList newList;
			for (auto c : cList)
				newList.push_back(new T_ConnectionType(*c));
			m_Connections.push_back(newList);
		}

		m_IsDirectionalGraph = other.m_IsDirectionalGraph;
		m_NextNodeIndex = other.m_NextNodeIndex;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline IGraph<T_NodeType, T_ConnectionType>::~IGraph()
	{
		Clear();
	}

	template<class T_NodeType, class T_ConnectionType>
	inline T_NodeType* IGraph<T_NodeType, T_ConnectionType>::GetNode(int idx) const
	{
		assert((idx < (int)m_Nodes.size()) && (idx >= 0) && "<Graph::GetNode>: invalid index");

		return m_Nodes[idx];
	}

	template<class T_NodeType, class T_ConnectionType>
	inline bool IGraph<T_NodeType, T_ConnectionType>::IsNodeValid(int idx) const
	{
		return (idx < (int)m_Nodes.size() && idx != invalid_node_index);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline T_ConnectionType* IGraph<T_NodeType, T_ConnectionType>::GetConnection(int from, int to) const
	{
		assert((from < (int)m_Nodes.size()) &&
			(from >= 0) &&
			m_Nodes[from]->GetIndex() != invalid_node_index &&
			"<Graph::GetConnection>: invalid 'from' index");

		assert((to < (int)m_Nodes.size()) &&
			(to >= 0) &&
			m_Nodes[to]->GetIndex() != invalid_node_index &&
			"<Graph::GetConnection>: invalid 'to' index");

		for (auto c : m_Connections[from])
		{
			if (c && c->GetTo() == to)
				return c;
		}

		return nullptr;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline vector<T_NodeType*> IGraph<T_NodeType, T_ConnectionType>::GetAllActiveNodes() const
	{
		vector<T_NodeType*> activeNodes{};
		for (auto n : m_Nodes)
			if (n->GetIndex() != invalid_node_index)
				activeNodes.push_back(n);

		return activeNodes;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline const std::list<T_ConnectionType*>& IGraph<T_NodeType, T_ConnectionType>::GetNodeConnections(int idx) const
	{
		assert((idx < (int)m_Nodes.size()) && (idx >= 0) && "<Graph::GetNode>: invalid index");

		return m_Connections[idx];
	}

	template<class T_NodeType, class T_ConnectionType>
	inline int IGraph<T_NodeType, T_ConnectionType>::AddNode(T_NodeType* pNode)
	{
		if (pNode->GetIndex() < (int)m_Nodes.size())
		{
			//make sure the client is not trying to add a pNode with the same ID as
			//a currently active pNode
			assert(m_Nodes[pNode->GetIndex()]->GetIndex() == invalid_node_index &&
				"<Graph::AddNode>: Attempting to add a node with a duplicate ID");

			m_Nodes[pNode->GetIndex()] = pNode;

			OnGraphModified(true, false);
			return m_NextNodeIndex;
		}
		else
		{
			//make sure the new pNode has been indexed correctly
			assert(pNode->GetIndex() == m_NextNodeIndex && "<Graph::AddNode>:invalid index");

			m_Nodes.push_back(pNode);
			m_Connections.push_back(ConnectionList());

			OnGraphModified(true, false);
			return m_NextNodeIndex++;
		}

	}

	template<class T_NodeType, class T_ConnectionType>
	inline void IGraph<T_NodeType, T_ConnectionType>::RemoveNode(int idx)
	{
		//Removes pNode by setting it's index to invalid_node_index 
		//This prevents the other indices from needing to be changed, however it can be reused when adding a new pNode with that index

		assert(idx < (int)m_Nodes.size() && "<Graph::RemoveNode>: invalid node index");

		//set this pNode's index to invalid_node_index
		m_Nodes[idx]->SetIndex(invalid_node_index);

		bool hadConnections = false;

		//if the graph is not directed remove all connections leading to this pNode and then
		//clear the connections leading from the pNode
		if (!m_IsDirectionalGraph)
		{
			//visit each neighbour and erase any connections leading to this pNode
			for (auto currentConnection = m_Connections[idx].begin();
				currentConnection != m_Connections[idx].end();
				++currentConnection)
			{
				for (auto& currentEdgeOnToNode = m_Connections[(*currentConnection)->GetTo()].begin();
					currentEdgeOnToNode != m_Connections[(*currentConnection)->GetTo()].end();
					++currentEdgeOnToNode)
				{
					if ((*currentEdgeOnToNode)->GetTo() == idx)
					{
						hadConnections = true;

						auto conPtr = *currentEdgeOnToNode;
						currentEdgeOnToNode = m_Connections[(*currentConnection)->GetTo()].erase(currentEdgeOnToNode);
						SAFE_DELETE(conPtr);

						break;
					}
				}
			}
		}

		//finally, clear this pNode's connections
		for (auto& connection : m_Connections[idx])
		{
			hadConnections = true;
			SAFE_DELETE(connection);
		}
		m_Connections[idx].clear();

		OnGraphModified(true, hadConnections);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void IGraph<T_NodeType, T_ConnectionType>::AddConnection(T_ConnectionType* pConnection)
	{
		//first make sure the from and to nodes exist within the graph 
		assert((pConnection->GetFrom() < m_NextNodeIndex) && (pConnection->GetTo() < m_NextNodeIndex) && (pConnection->GetTo() != pConnection->GetFrom()) &&
			"<Graph::AddConnection>: invalid node index");

		//make sure both nodes are active before adding the pConnection
		if ((m_Nodes[pConnection->GetTo()]->GetIndex() != invalid_node_index) &&
			(m_Nodes[pConnection->GetFrom()]->GetIndex() != invalid_node_index))
		{
			//add the pConnection, first making sure it is unique
			assert(IsUniqueConnection(pConnection->GetFrom(), pConnection->GetTo()) && "Connection already exists on this graph");

			m_Connections[pConnection->GetFrom()].push_back(pConnection);

			//if the graph is undirected we must add another pConnection in the opposite
			//direction
			if (!m_IsDirectionalGraph)
			{
				//check to make sure the pConnection is unique before adding
				if (IsUniqueConnection(pConnection->GetTo(), pConnection->GetFrom()))
				{
					T_ConnectionType* oppositeDirEdge = new T_ConnectionType();

					oppositeDirEdge->SetCost(pConnection->GetCost());
					oppositeDirEdge->SetTo(pConnection->GetFrom());
					oppositeDirEdge->SetFrom(pConnection->GetTo());

					m_Connections[pConnection->GetTo()].push_back(oppositeDirEdge);
				}
			}
		}

		OnGraphModified(false, true);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void IGraph<T_NodeType, T_ConnectionType>::RemoveConnection(int from, int to)
	{
		assert((from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) &&
			"<Graph::RemoveConnection>:invalid node index");

		auto conFromTo = GetConnection(from, to);
		auto conToFrom = GetConnection(to, from);

		if (!m_IsDirectionalGraph)
		{
			for (auto& curEdge = m_Connections[to].begin();
				curEdge != m_Connections[to].end();
				++curEdge)
			{
				if ((*curEdge)->GetTo() == from)
				{
					curEdge = m_Connections[to].erase(curEdge);
					break;
				}
			}
		}

		for (auto& curEdge = m_Connections[from].begin();
			curEdge != m_Connections[from].end();
			++curEdge)
		{
			if ((*curEdge)->GetTo() == to)
			{
				curEdge = m_Connections[from].erase(curEdge);
				break;
			}
		}

		SAFE_DELETE(conFromTo);
		SAFE_DELETE(conToFrom);

		OnGraphModified(false, true);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void IGraph<T_NodeType, T_ConnectionType>::RemoveConnection(T_ConnectionType* pConnection)
	{
		RemoveConnection(pConnection->GetFrom(), pConnection->GetTo());
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void IGraph<T_NodeType, T_ConnectionType>::RemoveConnectionsToAdjacentNodes(int idx)
	{
		// remove and delete connections from this pNode
		for (auto c : m_Connections[idx])
			delete c;
		m_Connections[idx].clear();

		// remove and delete connections from other nodes to this pNode
		auto isConnectionToThisNode = [idx](T_ConnectionType* pCon) { return pCon->GetTo() == idx; };
		for (auto& c : m_Connections)
		{
			list<T_ConnectionType*>::iterator foundIt;
			while ((foundIt = std::find_if(c.begin(), c.end(), isConnectionToThisNode)) != c.end())
			{
				delete* foundIt;
				c.erase(foundIt);
			}
		}

		OnGraphModified(false, true);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void IGraph<T_NodeType, T_ConnectionType>::SetConnectionCost(int from, int to, float cost)
	{
		//make sure the nodes given are valid
		assert((from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) &&
			"<Graph::SetEdgeCost>: invalid index");

		//visit each neighbour and erase any connections leading to this pNode
		for (auto& curEdge = m_Connections[from].begin();
			curEdge != m_Connections[from].end();
			++curEdge)
		{
			if (curEdge->GetTo() == to)
			{
				curEdge->SetCost(cost);
				break;
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	inline int IGraph<T_NodeType, T_ConnectionType>::GetNrOfActiveNodes() const
	{
		int count = 0;

		for (unsigned int n = 0; n < m_Nodes.size(); ++n)
			if (m_Nodes[n]->GetIndex() != invalid_node_index)
				++count;

		return count;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline int IGraph<T_NodeType, T_ConnectionType>::GetNrOfConnections() const
	{
		int tot = 0;

		for (auto curEdge = m_Connections.begin();
			curEdge != m_Connections.end();
			++curEdge)
		{
			tot += curEdge->size();
		}

		return tot;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void IGraph<T_NodeType, T_ConnectionType>::Clear()
	{
		for (auto& n : m_Nodes)
			SAFE_DELETE(n);
		m_Nodes.clear();

		for (auto& connectionList : m_Connections)
		{
			for (auto& connection : connectionList)
				SAFE_DELETE(connection);
		}
		m_Connections.clear();

		m_NextNodeIndex = 0;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void IGraph<T_NodeType, T_ConnectionType>::RemoveConnections()
	{
		for (auto& connectionList : m_Connections)
			connectionList.clear();
	}

	template<class T_NodeType, class T_ConnectionType>
	inline float IGraph<T_NodeType, T_ConnectionType>::GetNodeRadius(T_NodeType* pNode) const
	{
		return DEFAULT_NODE_RADIUS;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline bool IGraph<T_NodeType, T_ConnectionType>::IsUniqueConnection(int from, int to) const
	{
		for (auto c : m_Connections[from])
		{
			if (c->GetTo() == to)
			{
				return false;
			}
		}

		return true;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void IGraph<T_NodeType, T_ConnectionType>::CullInvalidEdges()
	{
		for (auto curEdgeList = m_Connections.begin(); curEdgeList != m_Connections.end(); ++curEdgeList)
		{
			for (auto& curEdge = (*curEdgeList).begin(); curEdge != (*curEdgeList).end(); ++curEdge)
			{
				if (m_Nodes[curEdge->GetTo()].GetIndex() == invalid_node_index ||
					m_Nodes[curEdge->GetFrom()].GetIndex() == invalid_node_index)
				{
					curEdge = (*curEdgeList).erase(curEdge);
				}
			}
		}
	}
}