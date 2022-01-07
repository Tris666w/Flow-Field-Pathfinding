/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
// Authors: Yosha Vandaele
/*=============================================================================*/
// EGraphConnectionTypes.h: Various edge types for graphs
/*=============================================================================*/

#pragma once

#include "EGraphEnums.h"

namespace Elite
{
	class GraphConnection
	{
	public:
		explicit GraphConnection(int from = invalid_node_index, int to = invalid_node_index, float cost = 1.f);
		virtual ~GraphConnection() = default;

		int GetFrom() const { return m_From; }
		void SetFrom(int newFrom) { m_From = newFrom; }

		int GetTo() const { return m_To; }
		void SetTo(int newTo) { m_To = newTo; }

		float GetCost() const { return m_Cost; }
		void SetCost(float newCost) { m_Cost = newCost; }

		bool IsValid() const { return (m_From != -1 && m_To != -1); }

		bool operator==(const GraphConnection& rhs) const;
		bool operator!=(const GraphConnection& rhs) const;

	protected:
		int m_From;
		int m_To;

		// the cost of traversing the edge
		float m_Cost;
	};

	class GraphConnection2D : public GraphConnection
	{
	public:
		GraphConnection2D(int from = invalid_node_index, int to = invalid_node_index, float cost = 1.f);
		virtual ~GraphConnection2D() = default;

		Color GetColor() const { return m_Color; }
		void SetColor(const Color& color) { m_Color = color; }

	protected:
		Color m_Color;
	};
}