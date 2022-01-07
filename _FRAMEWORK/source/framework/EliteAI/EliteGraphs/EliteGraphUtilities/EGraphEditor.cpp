#include "stdafx.h"
#include "EGraphEditor.h"


bool Elite::GraphEditor::UpdateGraph(GridGraph<GridTerrainNode, GraphConnection>* pGraph)
{
#pragma region UI
	//Extra Grid Terrain UI
	{ 
		//Setup
		int menuWidth = 115;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height/2.0f));
		ImGui::Begin("Grid Editing", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );
		ImGui::PushAllowKeyboardFocus(false);

		ImGui::Text("Terrain Type");
		ImGui::Indent();
		if (ImGui::Combo("", &m_SelectedTerrainType, "Ground\0Mud\0Water", 3))
		{

		}
		
		/*Spacing*/ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#pragma endregion


	//Check if clicked on grid
	auto mouseLeftData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
	auto mouseMotionData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseMotion);

	if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eLeft))
	{
		Vector2 mousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld({ (float)mouseLeftData.X, (float)mouseLeftData.Y });
		int idx = pGraph->GetNodeIdxAtWorldPos(mousePos);
		
		if (idx != invalid_node_index)
		{
			std::vector<TerrainType> terrainTypeVec{ TerrainType::Ground, TerrainType::Mud, TerrainType::Water };

			pGraph->GetNode(idx)->SetTerrainType(terrainTypeVec[m_SelectedTerrainType]);
			
			switch (terrainTypeVec[m_SelectedTerrainType])
			{
			case TerrainType::Water:
				pGraph->RemoveConnectionsToAdjacentNodes(idx);
				break;
			default:
				pGraph->RemoveConnectionsToAdjacentNodes(idx);
				pGraph->AddConnectionsToAdjacentCells(idx);
				break;
			}
			return true;
		}
	}

	return false;
}
