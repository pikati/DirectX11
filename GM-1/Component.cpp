#include "Component.h"
#include "LevelLoader.h"


Component* Component::GetComponent()
{
	//Component* c = new decltype(*this)();
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

//ここではプロパティを読み込むだけなのでユーザー定義のcomponentの場合はreturnでおｋだよ
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