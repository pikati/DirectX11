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

void Item::DrawInformation()
{
	std::string name = typeid(*this).name();
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(name.substr(6).c_str());

	const char* items[] = { "Strawberry", "Apple", "Banana" };
	int itemCurrent = (int)m_itemType;
	ImGui::Combo("ItemType", &itemCurrent, items, IM_ARRAYSIZE(items));
	m_itemType = (ItemType)itemCurrent;
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
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