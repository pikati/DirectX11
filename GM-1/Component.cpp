#include "Component.h"
#include "LevelLoader.h"
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"

void Component::Initialize()
{

}

void Component::Update()
{

}

void Component::Draw()
{

}

void Component::Finalize()
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

void Component::SetInspector()
{
	std::string name = typeid(*this).name();
	ImGui::Text(name.substr(6).c_str());
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