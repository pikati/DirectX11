#include "Hierarchy.h"
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"
#include "manager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Inspector.h"
#include <list>

void Hierarchy::Initialize()
{
    
}

void Hierarchy::Update()
{
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

    ImGui::Begin("Hierarchy");

	std::list<GameObject*>* objects = CManager::GetScene()->GetAllGameObject();

    for (int i = 0; i < LAYER_MAX; i++)
    {
        for (GameObject* object : objects[i])
        {
            if (ImGui::Button(object->name.c_str()))
            {
                Inspector::SetGameObject(object);
            }
        }
    }

    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void Hierarchy::Draw()
{

}

void Hierarchy::Finalize()
{

}