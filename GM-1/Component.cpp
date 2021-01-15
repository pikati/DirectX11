#include "Component.h"
#include "LevelLoader.h"
#include "imGui/imgui.h"

void Component::Initialize()
{

}

void Component::SystemInitialize()
{

}

void Component::Update()
{

}

void Component::SystemUpdate()
{

}

void Component::Draw()
{

}

void Component::Finalize()
{

}

void Component::SystemFinalize()
{
}

Component* Component::GetComponent()
{
	return this;
}

void Component::SetGameObject(GameObject* obj)
{
	gameObject = obj;
}

GameObject* Component::GetGameObject()
{
	return gameObject;
}

void Component::OnCollisionEnter(GameObject* obj)
{
	return;
}

void Component::OnCollisionExit(GameObject* obj)
{
	return;
}

void Component::DrawInformation()
{
	std::string name = typeid(*this).name();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(name.substr(6).c_str());

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Component::SetProperties(Component* c)
{
	return;
}

void Component::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetInt(inProp, "id", m_id);
	return;
}

void Component::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddInt(alloc, inProp, "id", m_id);
	return;
}

void Component::SetID(int n)
{
	m_id = n;
}

int Component::GetID()
{
	return m_id;
}