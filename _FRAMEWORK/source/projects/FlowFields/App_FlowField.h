#ifndef FLOWFIELD_APPLICATION_H
#define FLOWFIELD_APPLICATION_H
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "projects/FlowFields/FlowField.h" 
#include "framework/EliteInterfaces/EIApp.h"
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphUtilities\EGraphEditor.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphUtilities\EGraphRenderer.h"
class NavigationColliderElement;

class App_FlowField final : public IApp
{
public:
	App_FlowField();
	~App_FlowField() override;
	void Start() override;
	void Update(float deltaTime) override;
	void UpdateImGui();
	void Render(float deltaTime) const override;

private:
	Elite::Vector2 m_TrimWorldSize = { -1,-1 };
	//Grid
	static const int COLUMNS = 20;
	static const int ROWS = 10;
	unsigned int m_SizeCell = 20;
	Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>* m_pGridGraph;

	//Editor and Visualisation
	Elite::GraphEditor m_GraphEditor{};
	Elite::GraphRenderer m_GraphRenderer{};


	//Pathfinding
	FlowField m_FlowField{};
	int m_EndPathIndex = invalid_node_index;
};






#endif