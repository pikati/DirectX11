#include "Inspector.h"
#include "imGui/imgui.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "ClassDictionary.h"

GameObject* Inspector::m_object = nullptr;
std::vector<bool> Inspector::m_isDrawInfo;
bool Inspector::m_isComponent = false;

void Inspector::Initialize()
{

}

void Inspector::Update()
{

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);

	ImGui::Begin("Inspector");
	DispObjectInformation();

	if (ImGui::Button("AddComponent"))
	{
		m_isComponent = !m_isComponent;
	}

	if (m_isComponent)
	{
		DispAddComponentWindow();
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Inspector::Draw()
{

}

void Inspector::Finalize()
{
	m_object = nullptr;
}

void Inspector::DispObjectInformation()
{
	if (m_object == nullptr) return;

	if (ImGui::Button("Transform"))
	{
		m_isDrawInfo[0] = !m_isDrawInfo[0];
	}

	int count = 1;
	std::list<Component*> components = m_object->GetComponents();
	for (Component* c : components)
	{
		std::string name = typeid(*c).name();
		if (ImGui::Button(name.substr(6).c_str()))
		{
			m_isDrawInfo[count] = !m_isDrawInfo[count];
		}
		count++;
	}

	if (m_isDrawInfo[0])
	{
		DispTransform();
	}
	count = 1;
	for (Component* c : components)
	{
		if (m_isDrawInfo[count])
		{
			c->DrawInformation();
		}
		count++;
	}
}

void Inspector::DispTransform()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin("Transform");

	ImGui::Text("position");
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::InputFloat(" ", &m_object->transform->position.x, 0.01f, 1.0f, 5);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::InputFloat("  ", &m_object->transform->position.y, 0.01f, 1.0f, 5);
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::InputFloat("   ", &m_object->transform->position.z, 0.01f, 1.0f, 5);
	ImGui::Text("rotation");
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::InputFloat("    ", &m_object->transform->rotation.x, 0.01f, 1.0f, 5);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::InputFloat("     ", &m_object->transform->rotation.y, 1.01f, 1.0f, 5);
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::InputFloat("      ", &m_object->transform->rotation.z, 0.01f, 1.0f, 5);
	ImGui::Text("scale");
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::InputFloat("       ", &m_object->transform->scale.x, 0.01f, 1.0f, 5);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::InputFloat("        ", &m_object->transform->scale.y, 0.01f, 1.0f, 5);
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::InputFloat("         ", &m_object->transform->scale.z, 0.01f, 1.0f, 5);

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Inspector::DispAddComponentWindow()
{
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin("AddComponent");
	std::vector<std::string> componentName = ClassDictionary::GetComponentName();
	for (std::string str : componentName)
	{
		if (ImGui::Button(str.c_str()))
		{
			ClassDictionary::AddComponent(str, m_object);
			m_isDrawInfo.push_back(false);
		}
	}
	ImGui::End();
}

void Inspector::SetGameObject(GameObject* obj)
{
	m_object = obj;
	m_isDrawInfo.clear();
	int n = obj->GetComponents().size() + 1;
	for (int i = 0; i < n; i++)
	{
		m_isDrawInfo.push_back(false);
	}
}