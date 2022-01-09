#include "stdafx.h"
#include "App_FlowField.h"

#include "projects/FlowFields/FlowFieldFlock.h"
#include "projects/Movement/SteeringBehaviors/SquareObstacle.h"

App_FlowField::App_FlowField()
{
}

App_FlowField::~App_FlowField()
{
	for (auto obstacle : m_pObstacles)
	{
		SAFE_DELETE(obstacle);
	}
	SAFE_DELETE(m_pGridGraph);
	SAFE_DELETE(m_pFlock);
}

void App_FlowField::Start()
{
	//---------------------------------------------------
	//INITIALIZE WORLD AND CAMERA
	//---------------------------------------------------
	m_TrimWorldSize.x = static_cast<float>(COLUMNS) * static_cast<float>(m_SizeCell);
	m_TrimWorldSize.y = static_cast<float>(ROWS) * static_cast<float>(m_SizeCell);
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(125.f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(m_TrimWorldSize / 2.f);

	//-------------------------------------------------------------------
	//INITIALIZE GRID , TARGET AND FLOWFIELD
	//-------------------------------------------------------------------
	m_pGridGraph = new Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>(COLUMNS, ROWS, m_SizeCell,
		false, true, 1.f, 1.5f);
	GenerateWalls();
	GenerateMud();
	GenerateWater();
	m_EndPathIndex = Elite::randomInt(m_pGridGraph->GetAllNodes().size());
	m_FlowField.GenerateIntegrationAndFlowField(m_pGridGraph, m_EndPathIndex);

	//----------------------------------
	//INITIALIZE AGENT
	//----------------------------------
	m_pFlock = new FlowFieldFlock(m_pGridGraph,&m_pObstacles, COLUMNS, ROWS,
		static_cast<float>(m_SizeCell), m_AmountOfAgents, m_AgentSpeed);
}

void App_FlowField::Update(float deltaTime)
{
	//----------
	//INPUT
	//----------
	bool const middleMousePressed = INPUTMANAGER->IsMouseButtonUp(Elite::InputMouseButton::eMiddle);
	if (middleMousePressed)
	{
		Elite::MouseData mouseData = { INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eMiddle) };
		Elite::Vector2 mousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld({ (float)mouseData.X, (float)mouseData.Y });

		//Find closest node to click pos
		int closestNode = m_pGridGraph->GetNodeIdxAtWorldPos(mousePos);
		m_EndPathIndex = closestNode;
		m_FlowField.GenerateIntegrationAndFlowField(m_pGridGraph, m_EndPathIndex);
	}
	//----------
	//IMGUI
	//----------
	UpdateImGui();
	 
	//----------
	//UPDATE AGENTS
	//----------
	m_pFlock->Update(deltaTime);
}

void App_FlowField::UpdateImGui()
{
#ifdef PLATFORM_WINDOWS
#pragma region UI
	//UI
	{
		//Setup
		int menuWidth = 150;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
		ImGui::Begin("Flow Field Pathfinding", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::PushAllowKeyboardFocus(false);

		//Elements
		ImGui::Text("CONTROLS");
		ImGui::Indent();
		ImGui::Text("LMB: Set terrain");
		ImGui::Text("Middle Mouse: Set target");
		ImGui::Unindent();

		ImGui::Text("STATS");
		ImGui::Indent();
		ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Unindent();

		ImGui::Text("Debug visualization");
		ImGui::Indent();
		ImGui::Checkbox("Show Grid/Nodes", &dbNodes);
		ImGui::Checkbox("Show cost field", &dbCostFieldCosts);
		ImGui::Checkbox("Show integration field", &dbIntegrationFieldCosts);
		ImGui::Checkbox("Show directions", &dbDirections);
		ImGui::Unindent();

		ImGui::Text("Amount of agents");
		if (ImGui::InputInt("", &m_AmountOfAgents, 25))
			m_pFlock->ChangeAmountOfAgents(m_AmountOfAgents);

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#pragma endregion
#endif
}

void App_FlowField::Render(float deltaTime) const
{
	//--------------------------------
	//DRAW THE WORLD
	//--------------------------------
	std::vector<Elite::Vector2> points
	{
		{ 0, m_TrimWorldSize.y },
	{ m_TrimWorldSize.x, m_TrimWorldSize.y },
	{ m_TrimWorldSize.x, 0 },
	{ 0, 0 }
	};
	DEBUGRENDERER2D->DrawSolidPolygon(&points[0], 4, { 0.8f,0.8f,0.8f,0.4f }, 0.1f);

	m_GraphRenderer.RenderGraph(
		m_pGridGraph,
		dbNodes,
		dbCostFieldCosts,
		dbIntegrationFieldCosts,
		dbDirections,
		false
	);

	//Highlight the target node
	if (m_EndPathIndex != -1)
	{
		m_GraphRenderer.HighlightNodesRect(m_pGridGraph, { m_pGridGraph->GetNode(m_EndPathIndex) }, END_NODE_COLOR);
	}

}

void App_FlowField::ChangeAmountOfAgents()
{

}

void App_FlowField::GenerateWalls()
{

	CreateObstacle(142);
	CreateObstacle(122);
	CreateObstacle(102);
	CreateObstacle(82);
	CreateObstacle(62);


	CreateObstacle(45);
	CreateObstacle(46);
	CreateObstacle(47);
	CreateObstacle(48);
	CreateObstacle(49);

	CreateObstacle(170);
	CreateObstacle(171);
	CreateObstacle(172);

	CreateObstacle(34);
	CreateObstacle(35);
	CreateObstacle(36);
	CreateObstacle(56);
	CreateObstacle(76);
	CreateObstacle(96);

	CreateObstacle(177);
	CreateObstacle(157);
	CreateObstacle(156);

}

void App_FlowField::GenerateMud()
{
	m_pGridGraph->GetNode(143)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(123)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(103)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(83)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(63)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(66)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(67)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(68)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(69)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(88)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(104)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(124)->SetTerrainType(TerrainType::Mud);
	m_pGridGraph->GetNode(144)->SetTerrainType(TerrainType::Mud);

}
void App_FlowField::GenerateWater()
{
	m_pGridGraph->GetNode(113)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(133)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(132)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(153)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(154)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(155)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(135)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(134)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(114)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(115)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(116)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(117)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(95)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(94)->SetTerrainType(TerrainType::Water);
	m_pGridGraph->GetNode(93)->SetTerrainType(TerrainType::Water);

}
void App_FlowField::CreateObstacle(int idx)
{
	m_pGridGraph->GetNode(idx)->SetTerrainType(TerrainType::Wall);
	auto pos = m_pGridGraph->GetNodeWorldPos(idx);
	auto obstacle = new SquareObstacle(pos, static_cast<float>(m_SizeCell));
	m_pObstacles.push_back(obstacle);
}
