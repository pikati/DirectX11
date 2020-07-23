#pragma once
#include "GameObject.h"
#include "rapidjson/document.h"
class Component
{
protected:
	GameObject* gameObject = nullptr;
public:
	int m_sortingOrder = 1;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
	Component* GetComponent();
	void SetGameObject(GameObject* obj);
	GameObject* GetGameObject();
	virtual void LoadProperties(const rapidjson::Value& inProp);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp);

	template <typename T>
	T* GetComponent()
	{
		if (typeid(*this) == typeid(T))
		{
			return *this;
		}
	}
};

