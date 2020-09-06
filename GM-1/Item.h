#pragma once
#include "Component.h"

class SphereCollider;

enum ItemType
{
	Strawberry,
	Apple,
	Banana,
	Max
};


class Item : public Component
{
private:
	SphereCollider* m_collider = nullptr;
	ItemType m_itemType = Max;
public:
	void Initialize();
	void Update();
	ItemType GetItemType();
	void SetItemType(ItemType type);

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

