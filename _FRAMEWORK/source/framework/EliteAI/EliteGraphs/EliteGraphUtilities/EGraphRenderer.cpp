#include "stdafx.h"
#include "EGraphRenderer.h"

namespace Elite
{
	void GraphRenderer::RenderGraph(GridGraph<FlowFieldNode, GraphConnection>* pGraph, bool renderNodes,
		bool renderCostFieldCost, bool renderIntegrationCost, bool renderDirections, bool renderConnections) const
	{
		for (auto node : pGraph->GetAllActiveNodes())
		{
			if (renderNodes)
			{
				std::string costTxt = "";
				std::string integrationTxt = "";
				if (renderCostFieldCost)
					costTxt = GetCostFieldText(node);
				if (renderIntegrationCost)
					integrationTxt = GetIntegrationFieldText(node);

				int cellSize = pGraph->m_CellSize;

				Vector2 worldPos = pGraph->GetNodeWorldPos(node);
				RenderRectNode(worldPos, costTxt, integrationTxt, cellSize, GetNodeColor(node));

				if (renderDirections)
				{
					Vector2 dir = node->GetDirection();
					DEBUGRENDERER2D->DrawDirection(worldPos, dir, static_cast<float>(cellSize) / 2.f, { 0.f,1.f,0.f }, 0.1f);
				}
			}



			if (renderConnections)
			{
				//Connections
				for (auto con : pGraph->GetNodeConnections(node->GetIndex()))
				{
					std::string conTxt = "";

					RenderConnection(con, pGraph->GetNodeWorldPos(con->GetTo()), pGraph->GetNodeWorldPos(con->GetFrom()), conTxt, GetConnectionColor(con));
				}
			}
		}
	}

	void GraphRenderer::RenderCircleNode(Vector2 pos, std::string text /*= ""*/, float radius /*= 3.0f*/, Elite::Color col /*= DEFAULT_NODE_COLOR*/, float depth /*= 0.0f*/) const
	{
		DEBUGRENDERER2D->DrawSolidCircle(pos, radius, { 0,0 }, col, depth);
		const auto stringOffset = Vector2{ -0.5f, 1.f };
		DEBUGRENDERER2D->DrawString(pos + stringOffset, text.c_str());
	}

	void GraphRenderer::RenderRectNode(Vector2 pos, std::string text /*= ""*/, float width /* = 3.0f*/, Elite::Color col /*= DEFAULT_NODE_COLOR*/, float depth /*= 0.0f*/) const
	{
		Vector2 verts[4]
		{
			Vector2(pos.x - width / 2.0f, pos.y - width / 2.0f),
			Vector2(pos.x - width / 2.0f, pos.y + width / 2.0f),
			Vector2(pos.x + width / 2.0f, pos.y + width / 2.0f),
			Vector2(pos.x + width / 2.0f, pos.y - width / 2.0f)
		};

		DEBUGRENDERER2D->DrawSolidPolygon(&verts[0], 4, col, depth);

		const auto stringOffset = Vector2{ -0.5f, 1.f };
		DEBUGRENDERER2D->DrawString(pos + stringOffset, text.c_str());
	}

	void GraphRenderer::RenderRectNode(Vector2 pos, const std::string& costText, const std::string& integrationText, float width,
		Elite::Color col, float depth) const
	{
		Vector2 verts[4]
		{
			Vector2(pos.x - width / 2.0f, pos.y - width / 2.0f),
			Vector2(pos.x - width / 2.0f, pos.y + width / 2.0f),
			Vector2(pos.x + width / 2.0f, pos.y + width / 2.0f),
			Vector2(pos.x + width / 2.0f, pos.y - width / 2.0f)
		};

		DEBUGRENDERER2D->DrawSolidPolygon(&verts[0], 4, col, depth);

		const auto stringOffset = Vector2{ -0.5f, 1.f };
		DEBUGRENDERER2D->DrawString(pos + stringOffset, costText.c_str());
		DEBUGRENDERER2D->DrawString(pos - stringOffset, integrationText.c_str());

	}


	void GraphRenderer::RenderConnection(GraphConnection* con, Elite::Vector2 toPos, Elite::Vector2 fromPos, std::string text, Elite::Color col, float depth/*= 0.0f*/) const
	{
		auto center = toPos + (fromPos - toPos) / 2;

		DEBUGRENDERER2D->DrawSegment(toPos, fromPos, col, depth);
		DEBUGRENDERER2D->DrawString(center, text.c_str());
	}

	std::string GraphRenderer::GetCostFieldText(FlowFieldNode* pNode) const
	{
		std::stringstream ss;
		ss << std::fixed << pNode->GetCostFieldCost();
		return ss.str();
	}

	std::string GraphRenderer::GetIntegrationFieldText(FlowFieldNode* pNode) const
	{
		std::stringstream ss;
		ss << std::fixed << static_cast<int>(pNode->GetIntegrationCost());
		return ss.str();
	}
}
