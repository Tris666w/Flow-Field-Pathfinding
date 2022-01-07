#pragma once


#include "framework\EliteAI\EliteGraphs\EGraphNodeTypes.h"
#include "framework\EliteAI\EliteGraphs\EGraphConnectionTypes.h"
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"
#include "framework\EliteAI\EliteGraphs\EGraph2D.h"
#include  <type_traits>

namespace Elite 
{
	class GraphRenderer final
	{
	public:
		GraphRenderer() = default;
		~GraphRenderer() = default;

		template<class T_NodeType, class T_ConnectionType>
		void RenderGraph(IGraph<T_NodeType, T_ConnectionType>* pGraph, bool renderNodes, bool renderConnections, bool renderNodeTxt = true, bool renderConnectionTxt = true) const;

		template<class T_NodeType, class T_ConnectionType>
		void RenderGraph(GridGraph<T_NodeType, T_ConnectionType>* pGraph, bool renderNodes, bool renderNodeTxt, bool renderConnections, bool renderConnectionsCosts) const;

		template<class T_NodeType, class T_ConnectionType>
		void HighlightNodes(GridGraph<T_NodeType, T_ConnectionType>* pGraph, std::vector<T_NodeType*> path, Color col = HIGHLIGHTED_NODE_COLOR) const;

		void SetNumberPrintPrecision(int precision) { m_FloatPrintPrecision = precision; }

	private:
		void RenderCircleNode(Vector2 pos, std::string text = "", float radius = DEFAULT_NODE_RADIUS, Elite::Color col = DEFAULT_NODE_COLOR, float depth = 0.0f) const;
		void RenderRectNode(Vector2 pos, std::string text = "", float width = DEFAULT_NODE_RADIUS, Elite::Color col = DEFAULT_NODE_COLOR, float depth = 0.0f) const;
		void RenderConnection(GraphConnection* con, Elite::Vector2 toPos, Elite::Vector2 fromPos, std::string text, Elite::Color col = DEFAULT_CONNECTION_COLOR, float depth = 0.0f) const;

		// Get correct color/text depending on the pNode/pConnection type
		template<class T_NodeType, typename = typename enable_if<! is_base_of<GraphNode2D, T_NodeType>::value>::type>
		Elite::Color GetNodeColor(T_NodeType* pNode) const;
		Elite::Color GetNodeColor(GraphNode2D* pNode) const;
		Elite::Color GetNodeColor(GridTerrainNode* pNode) const;

		template<class T_ConnectionType>
		Elite::Color GetConnectionColor(T_ConnectionType* pConnection) const;
		Elite::Color GetConnectionColor(GraphConnection2D* pConnection) const;

		template<class T_NodeType>
		std::string GetNodeText(T_NodeType* pNode) const;
		std::string GetNodeText(InfluenceNode* pNode) const;

		template<class T_ConnectionType>
		std::string GetConnectionText(T_ConnectionType* pConnection) const;
	
		//C++ make the class non-copyable
		GraphRenderer(const GraphRenderer&) = delete;
		GraphRenderer& operator=(const GraphRenderer&) = delete;

		// variables
		int m_FloatPrintPrecision = 1;
	};


	template<class T_NodeType, class T_ConnectionType>
	void GraphRenderer::RenderGraph(
		IGraph<T_NodeType, T_ConnectionType>* pGraph, 
		bool renderNodes, 
		bool renderConnections, 
		bool renderNodeTxt /*= true*/, 
		bool renderConnectionTxt /*= true*/) const
	{
		for (auto node : pGraph->GetAllActiveNodes())
		{
			if (renderNodes)
			{
				std::string nodeTxt = "";
				if (renderNodeTxt)
					nodeTxt = GetNodeText(node);

				RenderCircleNode(pGraph->GetNodeWorldPos(node),	nodeTxt, DEFAULT_NODE_RADIUS, GetNodeColor(node));
			}
		
			if (renderConnections)
			{
				//Connections
				for (auto con : pGraph->GetNodeConnections(node->GetIndex()))
				{
					std::string conTxt = "";
					if (renderConnectionTxt)
						conTxt = GetConnectionText(con);

					RenderConnection(con, pGraph->GetNodeWorldPos(con->GetTo()), pGraph->GetNodeWorldPos(con->GetFrom()), conTxt, GetConnectionColor(con));
				}
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	void GraphRenderer::RenderGraph(
		GridGraph<T_NodeType, T_ConnectionType>* pGraph, 
		bool renderNodes, 
		bool renderNodeNumbers,
		bool renderConnections, 
		bool renderConnectionsCosts) const
	{
		if (renderNodes)
		{
			//Nodes/Grid
			for (auto r = 0; r < pGraph->m_NrOfRows; ++r)
			{
				for (auto c = 0; c < pGraph->m_NrOfColumns; ++c)
				{
					int idx = r * pGraph->m_NrOfColumns + c;
					Vector2 cellPos{ pGraph->GetNodeWorldPos(idx) };
					int cellSize = pGraph->m_CellSize;

					//Node
					std::string nodeTxt{};
					if (renderNodeNumbers)
						nodeTxt = GetNodeText(pGraph->GetNode(idx));

					RenderRectNode(cellPos, nodeTxt, float(cellSize), GetNodeColor(pGraph->GetNode(idx)), 0.1f);
				}
			}
		}

		if (renderConnections)
		{
			for (auto node : pGraph->GetAllActiveNodes())
			{
				//Connections
				for (auto con : pGraph->GetNodeConnections(node->GetIndex()))
				{
					std::string conTxt{ };
					if (renderConnectionsCosts)
						conTxt = GetConnectionText(con);

					RenderConnection(con, pGraph->GetNodeWorldPos(con->GetTo()), pGraph->GetNodeWorldPos(con->GetFrom()), conTxt, GetConnectionColor(con));
				}
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	void GraphRenderer::HighlightNodes(GridGraph<T_NodeType, T_ConnectionType>* pGraph, std::vector<T_NodeType*> path, Color col /*= HIGHLIGHTED_NODE_COLOR*/) const
	{
		for (auto node : path)
		{
			//Node
			RenderCircleNode(
				pGraph->GetNodeWorldPos(node),
				"",
				3.1f,
				col,
				-0.2f
			);
		}
	}

	template<class T_NodeType, typename>
	inline Elite::Color GraphRenderer::GetNodeColor(T_NodeType* pNode) const
	{
		return DEFAULT_NODE_COLOR;
	}

	inline Elite::Color GraphRenderer::GetNodeColor(GraphNode2D* pNode) const
	{
		return pNode->GetColor();
	}

	inline Elite::Color GraphRenderer::GetNodeColor(GridTerrainNode* pNode) const
	{
		return  pNode->GetColor();
	}

	template<class T_ConnectionType>
	inline Elite::Color GraphRenderer::GetConnectionColor(T_ConnectionType* connection) const
	{
		return DEFAULT_CONNECTION_COLOR;
	}

	inline Elite::Color GraphRenderer::GetConnectionColor(GraphConnection2D* connection) const
	{
		return connection->GetColor();
	}

	template<class T_NodeType>
	inline std::string GraphRenderer::GetNodeText(T_NodeType* pNode) const
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(m_FloatPrintPrecision) << pNode->GetIndex();
		return ss.str();
	}

	inline std::string GraphRenderer::GetNodeText(InfluenceNode* pNode) const
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(m_FloatPrintPrecision) << pNode->GetInfluence();
		return ss.str();
	}

	template<class T_ConnectionType>
	std::string GraphRenderer::GetConnectionText(T_ConnectionType* pConnection) const
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(m_FloatPrintPrecision) << pConnection->GetCost();
		return ss.str();
	}

}
