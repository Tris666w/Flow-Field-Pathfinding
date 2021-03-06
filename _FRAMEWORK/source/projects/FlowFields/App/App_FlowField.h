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

class ObstacleBase;
class SteeringAgent;
class FlowFieldFlock;

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
	bool dbIntegrationFieldCosts = false;
	bool dbDirections = false;

	//Pathfinding
	FlowField m_FlowField{};
	int m_EndPathIndex = invalid_node_index;

	//Agent
	void ChangeAmountOfAgents();
#ifdef _DEBUG
	int m_AmountOfAgents = 200;
#else
	int m_AmountOfAgents = 500;
#endif


	float m_AgentSpeed = 16.0f;
	FlowFieldFlock* m_pFlock = nullptr;


	//C++ make the class non-copyable
	App_FlowField(const App_FlowField&) = delete;
	App_FlowField& operator=(const App_FlowField&) = delete;

	//Map
	vector<ObstacleBase*> m_pObstacles = {};
	void GenerateWalls();
	void GenerateMud();
	void GenerateWater();

	void CreateObstacle(int idx);
};
#endif