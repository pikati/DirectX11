#include "Item.h"
#include "GameObject.h"
#include "SphereCollider.h"
#include "LevelLoader.h"
#include <vector>

void Item::Initialize()
{
	m_collider = gameObject->GetComponent<SphereCollider>();
}

void Item::OnCollisionEnter(GameObject* obj)
{
	if (obj->tag == "Player")
	{
		gameObject->Destroy();
	}
}

ItemType Item::GetItemType()
{
	return m_itemType;
}

void Item::SetItemType(ItemType type)
{
	m_itemType = type;
}

void Item::LoadProperties(const rapidjson::Value& inProp)
{
	int n;
	JsonHelper::GetInt(inProp, "itemType", n);
	m_itemType = (ItemType)n;
}

void Item::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddInt(alloc, inProp, "itemType", (int)m_itemType);
}