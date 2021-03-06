#include "EditorController.h"
#include "Inspector.h"
#include "imgui/imgui.h"
#include "manager.h"
#include "Scene.h"
#include "main.h"

bool EditorController::m_isPlay = false;

void EditorController::Update()
{
	bool isOldIsPlay = m_isPlay;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 2 - 100, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Once);

	ImGui::Begin("GameController");
	
	ImGui::Checkbox("Play", &m_isPlay);
	if (ImGui::Button("Reload"))
	{
		CManager::GetScene()->LoadScene(CManager::GetScene()->GetSceneName());
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	if (!isOldIsPlay && m_isPlay)
	{
		CManager::GetScene()->PlayInitialize();
		//Inspector::DeleteObject();
	}
	else if (isOldIsPlay && !m_isPlay)
	{
		CManager::GetScene()->PlayFinalize();
	}
}

bool EditorController::IsPlay()
{
	return m_isPlay;
}