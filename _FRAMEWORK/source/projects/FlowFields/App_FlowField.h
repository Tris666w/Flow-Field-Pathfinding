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

class RectObstacle;
class SteeringAgent;
class FlowFieldSteering;

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
	Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>* m_pGridGraph{};

	//Editor and Visualisation
	Elite::GraphEditor m_GraphEditor{};
	Elite::GraphRenderer m_GraphRenderer{};

	//DebugRendering
	bool dbNodes = false;
	bool dbCostFieldCosts = false;
	bool dbIntegrationFieldCosts = true;
	bool dbDirections = false;

	//Pathfinding
	FlowField m_FlowField{};
	int m_EndPathIndex = invalid_node_index;

	//Agent
	vector<SteeringAgent*> m_pAgentVector = {};
	int m_AmountOfAgents = 50;
	FlowFieldSteering* m_pFlowFieldBehavior = nullptr;
	float m_AgentSpeed = 16.0f;

	void ChangeAmountOfAgents();

	//C++ make the class non-copyable
	App_FlowField(const App_FlowField&) = delete;
	App_FlowField& operator=(const App_FlowField&) = delete;

	//Map
	vector<RectObstacle*> m_pObstacles = {};
	void GenerateWalls();
	void CreateObstacle(int idx);
};






#endif