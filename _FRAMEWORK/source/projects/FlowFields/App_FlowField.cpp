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
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(55.0f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(m_TrimWorldSize / 1.5f, m_TrimWorldSize / 2));

	m_pGridGraph = new Elite::GridGraph<Elite::GridTerrainNode, Elite::GraphConnection>(COLUMNS, ROWS, m_SizeCell, false, false, 1.f, 1.5f);
	
}

void App_FlowField::Update(float deltaTime)
{
	UpdateImGui();
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


		ImGui::Text("STATS");
		ImGui::Indent();
		ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
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
		false,
		false,
		false
	);


}
