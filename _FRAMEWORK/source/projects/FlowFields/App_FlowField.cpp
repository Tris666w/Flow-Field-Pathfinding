#include "stdafx.h"
#include "App_FlowField.h"

App_FlowField::App_FlowField()
{
}

App_FlowField::~App_FlowField()
{
	SAFE_DELETE(m_pGridGraph);

}

void App_FlowField::Start()
{

	m_TrimWorldSize.x = static_cast<float>(COLUMNS) * static_cast<float>(m_SizeCell);
	m_TrimWorldSize.y = static_cast<float>(ROWS) * static_cast<float>(m_SizeCell);
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(125.f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(m_TrimWorldSize / 2.f);

	m_pGridGraph = new Elite::GridGraph<Elite::FlowFieldNode, Elite::GraphConnection>(COLUMNS, ROWS, m_SizeCell, false, true, 1.f, 1.5f);

	m_EndPathIndex = Elite::randomInt(m_pGridGraph->GetAllNodes().size());

	m_FlowField.GenerateIntegrationAndFlowField(m_pGridGraph, m_EndPathIndex);
}

void App_FlowField::Update(float deltaTime)
{
	//INPUT
	bool const rightMousePressed = INPUTMANAGER->IsMouseButtonUp(Elite::InputMouseButton::eRight);
	if (rightMousePressed)
	{
		Elite::MouseData mouseData = { INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eRight) };
		Elite::Vector2 mousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld({ (float)mouseData.X, (float)mouseData.Y });

		//Find closest node to click pos
		int closestNode = m_pGridGraph->GetNodeIdxAtWorldPos(mousePos);
		m_EndPathIndex = closestNode;
		m_FlowField.GenerateIntegrationAndFlowField(m_pGridGraph, m_EndPathIndex);

	}


	//ImGui
	UpdateImGui();

	//Update grid
	if (m_GraphEditor.UpdateGraph(m_pGridGraph))
	{
		m_FlowField.GenerateIntegrationAndFlowField(m_pGridGraph, m_EndPathIndex);
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
		ImGui::Text("RMB: Set target");
		ImGui::Unindent();

		ImGui::Text("STATS");
		ImGui::Indent();
		ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Unindent();

		ImGui::Text("Debug visualization");
		ImGui::Indent();
		ImGui::Checkbox("Show cost field", &dbCostFieldCosts);
		ImGui::Checkbox("Show integration field", &dbIntegrationFieldCosts);
		ImGui::Checkbox("Show directions", &dbDirections);
		ImGui::Checkbox("Show connections", &dbConnections);
		ImGui::Unindent();

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#pragma endregion
#endif
}

void App_FlowField::Render(float deltaTime) const
{
	m_GraphRenderer.RenderGraph(
		m_pGridGraph,
		true,
		dbCostFieldCosts,
		dbIntegrationFieldCosts,
		dbDirections,
		dbConnections
	);

	//Highlight the target node
	m_GraphRenderer.HighlightNodesRect(m_pGridGraph, { m_pGridGraph->GetNode(m_EndPathIndex) }, END_NODE_COLOR);

}
