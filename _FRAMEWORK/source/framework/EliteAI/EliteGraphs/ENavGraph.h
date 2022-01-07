#pragma once

#include "framework/EliteAI/EliteGraphs/EGraph2D.h"
#include "framework/EliteAI/EliteGraphs/EGraphConnectionTypes.h"
#include "framework/EliteAI/EliteGraphs/EGraphNodeTypes.h"

namespace Elite
{
	class NavGraph final: public Graph2D<NavGraphNode, GraphConnection2D>
	{
	public:
		NavGraph(const Polygon& baseMesh, float playerRadius );
		~NavGraph();

		int GetNodeIdxFromLineIdx(int lineIdx) const;
		Polygon* GetNavMeshPolygon() const;

	private:
		//--- Datamembers ---
		Polygon* m_pNavMeshPolygon = nullptr; //Polygon that represents navigation mesh

		void CreateNavigationGraph();


	private:
		NavGraph(const NavGraph& other) = delete;
		NavGraph& operator=(const NavGraph& other) = delete;
		NavGraph(NavGraph&& other) = delete;
		NavGraph& operator=(NavGraph&& other) = delete;
		
	};
}

