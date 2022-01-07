#include "stdafx.h"
#include "SpacePartitioning.h"
#include "projects\Movement\SteeringBehaviors\SteeringAgent.h"

// --- Cell ---
// ------------
Cell::Cell(float left, float bottom, float width, float height)
{
	boundingBox.bottomLeft = Elite::Vector2{ left, bottom };
	boundingBox.width = width;
	boundingBox.height = height;
}

std::vector<Elite::Vector2> Cell::GetRectPoints() const
{
	auto left = boundingBox.bottomLeft.x;
	auto bottom = boundingBox.bottomLeft.y;
	auto width = boundingBox.width;
	auto height = boundingBox.height;

	std::vector<Elite::Vector2> rectPoints =
	{
		{ left , bottom  },
		{ left , bottom + height  },
		{ left + width , bottom + height },
		{ left + width , bottom  },
	};

	return rectPoints;
}

// --- Partitioned Space ---
// -------------------------
CellSpace::CellSpace(float width, float height, int rows, int cols, int maxEntities)
	: m_SpaceWidth(width)
	, m_SpaceHeight(height)
	, m_NrOfRows(rows)
	, m_NrOfCols(cols)
	, m_Neighbors(maxEntities)
	, m_NrOfNeighbors(0)
{
	m_CellWidth = m_SpaceWidth / static_cast<float>(m_NrOfCols);
	m_CellHeight = m_SpaceHeight / static_cast<float>(m_NrOfRows);

	m_Neighbors.reserve(maxEntities);
	m_Neighbors.resize(maxEntities);

	m_NeighbouringCellIndexes.reserve(m_NrOfCols * m_NrOfRows);
	m_NeighbouringCellIndexes.resize(m_NrOfCols * m_NrOfRows);

	for (int row = 0; row < m_NrOfRows; ++row)
	{
		for (int col = 0; col < m_NrOfCols; ++col)
		{
			float const left = static_cast<float>(col) * m_CellWidth;
			float const bottom = static_cast<float>(row) * m_CellHeight;
			m_Cells.emplace_back(Cell(left, bottom, m_CellWidth, m_CellHeight));
		}
	}
}

void CellSpace::AddAgent(SteeringAgent* agent)
{
	int const index{ PositionToIndex(agent->GetPosition()) };
	m_Cells[index].agents.push_back(agent);
}

void CellSpace::UpdateAgentCell(SteeringAgent* agent, Elite::Vector2 oldPos)
{
	int const previousIndex{ PositionToIndex(oldPos) };
	int const newIndex{ PositionToIndex(agent->GetPosition()) };

	if (previousIndex == newIndex)
		return;
	else
	{
		m_Cells[previousIndex].agents.remove(agent);
		AddAgent(agent);
	}
}

void CellSpace::RegisterNeighbors(SteeringAgent* agent, float queryRadius)
{
	m_NrOfNeighbors = 0;
	auto const agentPos = agent->GetPosition();

	GetNeighboringCellsIndexes(agentPos, queryRadius);

	for (int index = 0; index < m_NrOfNeighborCells; ++index)
	{
		for (SteeringAgent* currentAgent : m_Cells[m_NeighbouringCellIndexes[index]].agents)
		{
			if (agentPos != currentAgent->GetPosition())
			{
				if (Distance(agentPos, currentAgent->GetPosition()) <=  queryRadius)
				{
					m_Neighbors[m_NrOfNeighbors] = currentAgent;
					++m_NrOfNeighbors;
				}
			}
		}
	}
}

void CellSpace::GetNeighboringCellsIndexes(const Elite::Vector2& targetPos, float queryRadius)
{
	Elite::Rect const queryRect({ targetPos.x - queryRadius, targetPos.y - queryRadius }, 2 * queryRadius, 2 * queryRadius);
	m_NrOfNeighborCells = 0;
	for (int index = 0; index < static_cast<int>(m_Cells.size()); ++index)
	{
		if (IsOverlapping(queryRect, m_Cells[index].boundingBox))
		{
			m_NeighbouringCellIndexes[m_NrOfNeighborCells] = index;
			++m_NrOfNeighborCells;
		}
	}
}


void CellSpace::RenderCells() const
{
	for (const auto& cell : m_Cells)
	{
		Elite::Polygon rect(cell.GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(&rect, Elite::Color{ 1.f,0.f,0.f }, 1.f);

		std::string amount{ to_string(cell.agents.size()) };
		DEBUGRENDERER2D->DrawString(Elite::Vector2{ rect.GetPosVertMinXPos(),rect.GetPosVertMaxYPos() }, amount.c_str());
	}
}

void CellSpace::DebugRenderNeighboringCells()
{
	for (int const m_NeighboringCellIndex : m_NeighbouringCellIndexes)
	{
		Elite::Polygon rect(m_Cells[m_NeighboringCellIndex].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(&rect, Elite::Color{ 0.f,0.f,0.f }, 1.f);
	}
}

void CellSpace::RenderBoundingBox(const Elite::Vector2& targetPos, float queryRadius)
{
	Elite::Rect const queryRect({ targetPos.x - queryRadius, targetPos.y - queryRadius }, 2 * queryRadius, 2 * queryRadius);

	std::vector<Elite::Vector2> vertices{};
	auto const botleft = queryRect.bottomLeft;
	vertices.push_back(botleft);
	vertices.emplace_back(botleft.x, botleft.y + queryRect.height);
	vertices.emplace_back(botleft.x + queryRect.width, botleft.y + queryRect.height);
	vertices.emplace_back(botleft.x + queryRect.width, botleft.y);

	Elite::Polygon rect(vertices);
	DEBUGRENDERER2D->DrawPolygon(&rect, Elite::Color{ 0.f,0.f,0.f }, 1.f);
}

int CellSpace::PositionToIndex(const Elite::Vector2 pos) const
{
	int col = static_cast<int>(pos.x) / static_cast<int>(m_CellWidth);
	int row = static_cast<int>(pos.y) / static_cast<int>(m_CellHeight);

	col = Elite::Clamp(col, 0, m_NrOfCols - 1);
	row = Elite::Clamp(row, 0, m_NrOfRows - 1);

	return row * m_NrOfCols + col;
}
