#include "stdafx.h"
#include "App_FlowField.h"

#include "FlowFieldSteering.h"
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "projects/FlowFields/RectObstacle.h"

App_FlowField::App_FlowField()
{
}

App_FlowField::~App_FlowField()
{
	for (auto agent : m_pAgentVector)
	{
		SAFE_DELETE(agent);
	}
	for (auto obstacle : m_pObstacles)
	{
		SAFE_DELETE(obstacle);
	}

	SAFE_DELETE(m_pGridGraph);
	SAFE_DELETE(m_pFlowFieldBehavior);
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
	m_EndPathIndex = Elite::randomInt(m_pGridGraph->GetAllNodes().size());
	m_FlowField.GenerateIntegrationAndFlowField(m_pGridGraph, m_EndPathIndex);


	//---------------------------------------------------
	//INITIALIZE AGENT AND STEERING
	//---------------------------------------------------
	m_pFlowFieldBehavior = new FlowFieldSteering(m_pGridGraph);
	for (int index = 0; index < m_AmountOfAgents; ++index)
	{
		auto agent = new SteeringAgent();
		m_pAgentVector.push_back(agent);
		agent->SetSteeringBehavior(m_pFlowFieldBehavior);
		agent->SetMaxLinearSpeed(m_AgentSpeed);
		agent->SetAutoOrient(true);
		agent->SetMass(0.1f);
		agent->SetPosition({ Elite::randomFloat(0,m_TrimWorldSize.x),Elite::randomFloat(0,m_TrimWorldSize.y) });
	}
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
	bool const rightMousePressed = INPUTMANAGER->IsMouseButtonUp(Elite::InputMouseButton::eRight);
	if (rightMousePressed)
	{
		Elite::MouseData mouseData = { INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eRight) };
		Elite::Vector2 mousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld({ (float)mouseData.X, (float)mouseData.Y });

		//Find closest node to click pos
		int closestNode = m_pGridGraph->GetNodeIdxAtWorldPos(mousePos);
		peach::Logger::LogInfo(std::to_string(closestNode));
	}
	//----------
	//IMGUI
	//----------
	UpdateImGui();

	//----------
	//UPDATE GRID
	//----------
	if (m_GraphEditor.UpdateGraph(m_pGridGraph))
	{
		m_FlowField.GenerateIntegrationAndFlowField(m_pGridGraph, m_EndPathIndex);
	}

	//----------
	//UPDATE AGENTS
	//----------
	for (const auto agent : m_pAgentVector)
	{
		agent->Update(deltaTime);
		agent->TrimToWorld({ 0,0 }, m_TrimWorldSize);
	}
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
		if (ImGui::InputInt("", &m_AmountOfAgents))
			ChangeAmountOfAgents();

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
	m_GraphRenderer.HighlightNodesRect(m_pGridGraph, { m_pGridGraph->GetNode(m_EndPathIndex) }, END_NODE_COLOR);

}

void App_FlowField::ChangeAmountOfAgents()
{
	int vectorSize = static_cast<int>(m_pAgentVector.size());

	if (m_AmountOfAgents == vectorSize || (vectorSize <= 0 && m_AmountOfAgents <= 0))
		return;


	if (m_AmountOfAgents > vectorSize)
	{
		while (m_AmountOfAgents > vectorSize)
		{
			auto agent = new SteeringAgent();
			m_pAgentVector.push_back(agent);
			agent->SetSteeringBehavior(m_pFlowFieldBehavior);
			agent->SetMaxLinearSpeed(m_AgentSpeed);
			agent->SetAutoOrient(true);
			agent->SetMass(0.1f);
			agent->SetPosition({ Elite::randomFloat(0,m_TrimWorldSize.x),Elite::randomFloat(0,m_TrimWorldSize.y) });

			++vectorSize;
		}
		return;
	}


	if (m_AmountOfAgents < vectorSize)
	{
		int const agentsDiff = vectorSize - m_AmountOfAgents;

		for (int i = 0; i < agentsDiff; i++)
		{
			auto agent = m_pAgentVector.front();
			std::_Erase_remove(m_pAgentVector, agent);
			SAFE_DELETE(agent)

		}
	}
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

void App_FlowField::CreateObstacle(int idx)
{
	m_pGridGraph->GetNode(idx)->SetTerrainType(TerrainType::Wall);
	auto pos = m_pGridGraph->GetNodeWorldPos(idx);
	Elite::Vector2 dimension = { static_cast<float>(m_SizeCell) ,static_cast<float>(m_SizeCell) };
	auto obstacle = new RectObstacle(pos, dimension);
	m_pObstacles.push_back(obstacle);
}
