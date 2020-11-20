#include "Inspector.h"
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"

GameObject* Inspector::m_object = nullptr;

void Inspector::Initialize()
{

}

void Inspector::Update()
{
    
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
    ImGui::SetNextWindowPos(ImVec2(1500, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);

    ImGui::Begin("Inspector");

    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
    
    DispObjectInformation();

    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void Inspector::Draw()
{

}

void Inspector::Finalize()
{

}

void Inspector::DispObjectInformation()
{
	if (m_object == nullptr) return;
    if (ImGui::TreeNode("Transform"))
    {
        ImGui::Text("position");
        ImGui::InputFloat("x", &m_object->transform->position.x, 0.01f, 1.0f, 5);
        ImGui::InputFloat("y", &m_object->transform->position.y, 0.01f, 1.0f, 5);
        ImGui::InputFloat("z", &m_object->transform->position.z, 0.01f, 1.0f, 5);
        ImGui::Text("rotation");
        ImGui::InputFloat("x ", &m_object->transform->rotation.x, 0.01f, 1.0f, 5);
        ImGui::InputFloat("y ", &m_object->transform->rotation.y, 0.01f, 1.0f, 5);
        ImGui::InputFloat("z ", &m_object->transform->rotation.z, 0.01f, 1.0f, 5);
        ImGui::Text("scale");
        ImGui::InputFloat("x  ", &m_object->transform->scale.x, 0.01f, 1.0f, 5);
        ImGui::InputFloat("y  ", &m_object->transform->scale.y, 0.01f, 1.0f, 5);
        ImGui::InputFloat("z  ", &m_object->transform->scale.z, 0.01f, 1.0f, 5);

        ImGui::TreePop();
    }
    std::list<Component*> components = m_object->GetComponents();
    for (Component* c : components)
    {
        c->SetInspector();
    }
}

void Inspector::SetGameObject(GameObject* obj)
{
	m_object = obj;
}