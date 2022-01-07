#pragma once
#include <numeric>

namespace Elite
{
	template <class T_NodeType, class T_ConnectionType>
	class AStar
	{
	public:
		AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			NodeRecord(T_NodeType* pN, T_ConnectionType* pCon, float eTotalCost)
				:pNode(pN),
				pConnection(pCon),
				estimatedTotalCost(eTotalCost)
			{
			}

			T_NodeType* pNode = nullptr;
			T_ConnectionType* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};

			NodeRecord operator+(const NodeRecord& rRecord)
			{
				NodeRecord result{};
				result.costSoFar = this->costSoFar + rRecord.costSoFar;
				return result;
			}
		};

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);

	private:
		float GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const;

		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
		Heuristic m_HeuristicFunction;
	};

	template <class T_NodeType, class T_ConnectionType>
	AStar<T_NodeType, T_ConnectionType>::AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction)
		: m_pGraph(pGraph)
		, m_HeuristicFunction(hFunction)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> AStar<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pGoalNode)
	{
		vector<T_NodeType*> path;
		vector<NodeRecord> openList;
		vector<NodeRecord> closedList;
		NodeRecord currentRecord{ pStartNode,nullptr,GetHeuristicCost(pStartNode,pGoalNode) };

		openList.push_back(currentRecord);

		while (!openList.empty())
		{
			//Get record with lowest F-cost
			currentRecord = *(std::min_element(openList.begin(), openList.end()));

			//Check if it points to the goal, if yes ->stop the loop
			if (currentRecord.pNode == pGoalNode)
				break;

			//Get all connections from this nodes
			auto connections = m_pGraph->GetNodeConnections(currentRecord.pNode);

			//for every connection
			for (auto currentConnection : connections)
			{
				//Calculate the total cost so far (G-cost)
				float gCost = currentRecord.costSoFar + currentConnection->GetCost();

				bool checkSucceeded = false;

				//2.D Check the closed list and if a connection to the connections endNode already exist in the closed list
				auto closedIt = std::find_if(closedList.begin(), closedList.end(),
					[currentConnection](NodeRecord listRecord) ->bool {
						if (!listRecord.pConnection)
							return false;
						if (currentConnection->GetTo() == listRecord.pConnection->GetTo())
							return true;
						else
							return false; }
				);
				if (closedIt != closedList.end())
				{
					checkSucceeded = true;
					//Check if the already existing connection is cheaper 
					if (closedIt->costSoFar < gCost)
					{
						//continue to the next connection 
						continue;
					}
					else
					{
						//remove it from the closedList (so it can be replaced)
						std::_Erase_remove(closedList, *(closedIt));
					}
				}

				//2. E If 2.D check failed, check if any of those connections lead to a node already on the open list
				if (!checkSucceeded)
				{
					//Check the open list (pNode) and if a connection to the connections endNode already exist in the open list
					auto openIt = std::find_if(openList.begin(), openList.end(),
						[currentConnection](NodeRecord listRecord) ->bool {
							if (!listRecord.pConnection)
								return false;
							if (currentConnection->GetTo() == listRecord.pConnection->GetTo())
								return true;
							else
								return false; }
					);

					if (openIt != openList.end())
					{
						//Check if the already existing connection is cheaper 
						if (openIt->costSoFar < gCost)
						{
							//continue to the next connection 
							continue;
						}
						else
						{
							//remove it from the openList (so it can be replaced)
							std::_Erase_remove(openList, *(openIt));
						}
					}
				}

				//2. F At this point any expensive connection should be removed(if it existed).We create a new nodeRecord and add it to the openList
				auto node = m_pGraph->GetNode(currentConnection->GetTo());
				NodeRecord newRecord{ node,currentConnection,gCost + GetHeuristicCost(node,pGoalNode) };
				newRecord.costSoFar = gCost;
				openList.push_back(newRecord);
			}
			//2. G Remove NodeRecord from the openList and add it to the closedList
			std::_Erase_remove(openList, currentRecord);
			closedList.push_back(currentRecord);
		}

		//3. Reconstruct path from last connection to start node

		//Track back from the currentRecord until the node of the record is the startnode of the overall path
		//Add each time the node of the currentRecord to the path
		//Look in the closedList for a record where pNode == the currentRecords’ connections’ startNode

		while (currentRecord.pNode != pStartNode)
		{
			path.push_back(currentRecord.pNode);

			auto const it = std::find_if(closedList.begin(), closedList.end(), [currentRecord, this](const NodeRecord& other)
				{
					if (other.pNode == m_pGraph->GetNode(currentRecord.pConnection->GetFrom()))
						return true;
					return false;
				});
			if (it != closedList.end())
			{
				currentRecord = *(it);
			}
			else
			{
				peach::Logger::LogWarning("EAStar.h, AStar::FindPath(), No next NodeRecord found (line:171)");
				break;
			}
		}
		std::reverse(path.begin(), path.end());

		return path;
	}

	template <class T_NodeType, class T_ConnectionType>
	float Elite::AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const
	{
		Vector2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}
}