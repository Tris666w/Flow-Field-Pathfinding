#include "stdafx.h"
#include "EGraphRenderer.h"

namespace Elite
{
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

	

	void GraphRenderer::RenderConnection(GraphConnection* con, Elite::Vector2 toPos, Elite::Vector2 fromPos, std::string text, Elite::Color col, float depth/*= 0.0f*/) const
	{
		auto center = toPos + (fromPos - toPos) / 2;

		DEBUGRENDERER2D->DrawSegment(toPos, fromPos, col, depth);
		DEBUGRENDERER2D->DrawString(center, text.c_str());
	}
}