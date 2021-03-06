#include "Inspector.h"
#include "imGui/imgui.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "ClassDictionary.h"
#include "manager.h"
#include "Scene.h"
#include <string>

ActiveObjInfo* Inspector::m_object = nullptr;
std::vector<bool> Inspector::m_isDrawInfo;
bool Inspector::m_isComponent = false;
bool Inspector::m_isDeleted = false;

void Inspector::Initialize()
{
	m_object = new ActiveObjInfo();
	m_object->obj = nullptr;
}

void Inspector::Update()
{
	if (!m_object) return;
	if (!m_object->obj) return;
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

	if (ImGui::Button("Copy"))
	{
		Copy();
	}

	if (ImGui::Button("Delete"))
	{
		DeleteObject();
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
	if (m_object)
	{
		if (m_object->obj)
		{
			m_object->obj = nullptr;
		}
		delete m_object;
		m_object = nullptr;
	}
}

void Inspector::DispObjectInformation()
{
	if (m_object->obj == nullptr) return;
	char name[256];
	char tag[64];
	int layer;
	m_isDeleted = false;
	strcpy_s(name, 256, m_object->obj->name.c_str());
	ImGui::InputText("name", name, 256);
	m_object->obj->name = name;
	strcpy_s(tag, 64, m_object->obj->tag.c_str());
	ImGui::InputText("tag", tag, 64);
	m_object->obj->tag = tag;
	layer = m_object->obj->layer;
	ImGui::InputInt("layer", &layer);
	if (layer >= LAYER_MAX)
	{
		layer = LAYER_MAX - 1;
	}
	m_object->obj->layer = layer;

	if (ImGui::Button("Transform"))
	{
		m_isDrawInfo[0] = !m_isDrawInfo[0];
	}

	int count = 1;
	std::list<Component*> components = m_object->obj->GetComponents();
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
			if (m_isDeleted) return;
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
	ImGui::InputFloat(" ", &m_object->obj->transform->position.x, 0.01f, 1.0f, 5);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::InputFloat("  ", &m_object->obj->transform->position.y, 0.01f, 1.0f, 5);
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::InputFloat("   ", &m_object->obj->transform->position.z, 0.01f, 1.0f, 5);
	ImGui::Text("rotation");
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::InputFloat("    ", &m_object->obj->transform->rotation.x, 0.01f, 1.0f, 5);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::InputFloat("     ", &m_object->obj->transform->rotation.y, 1.01f, 1.0f, 5);
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::InputFloat("      ", &m_object->obj->transform->rotation.z, 0.01f, 1.0f, 5);
	ImGui::Text("scale");
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::InputFloat("       ", &m_object->obj->transform->scale.x, 0.01f, 1.0f, 5);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::InputFloat("        ", &m_object->obj->transform->scale.y, 0.01f, 1.0f, 5);
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::InputFloat("         ", &m_object->obj->transform->scale.z, 0.01f, 1.0f, 5);

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Inspector::DispAddComponentWindow()
{
	bool batsu = true;
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin("AddComponent", &batsu);
	std::vector<std::string> componentName = ClassDictionary::GetComponentName();
	for (std::string str : componentName)
	{
		if (ImGui::Button(str.c_str()))
		{
			Component* c = ClassDictionary::AddComponent(str, m_object->obj);
			c->SystemInitialize();
			m_isDrawInfo.push_back(false);
			m_isComponent = false;
		}
	}
	ImGui::End();
}

void Inspector::SetGameObject(GameObject* obj, int layer, int index)
{
	if (!m_object)
	{
		m_object = new ActiveObjInfo();
	}
	else
	{
		if (m_object->obj)
		{
			m_object->obj->SetActiveGizmo(false);
		}
	}
	m_object->obj = obj;
	m_object->layer = layer;
	m_object->index = index;
	m_isDrawInfo.clear();
	int n = obj->GetComponents().size() + 1;
	for (int i = 0; i < n; i++)
	{
		m_isDrawInfo.push_back(false);
	}
	m_object->obj->SetActiveGizmo(true);
}

void Inspector::DeleteObject()
{
	CManager::GetScene()->DeleteObject(m_object->layer, m_object->index);
	m_object->obj->SetActiveGizmo(false);
	m_object->obj = nullptr;
}

void Inspector::DeleteInformation()
{
	for (unsigned int i = 1; i < m_isDrawInfo.size(); i++)
	{
		if (m_isDrawInfo[i] == true)
		{
			m_isDrawInfo.erase(m_isDrawInfo.begin() + i);
			m_isDeleted = true;
			return;
		}
	}
}

void Inspector::Copy()
{
	if (!m_object->obj) return;
	GameObject* obj = new GameObject(*m_object->obj);
	CManager::GetScene()->AddGameObject(obj, false);
	m_object->obj = obj;
}