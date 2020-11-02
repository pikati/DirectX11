#pragma once
#include "GameObject.h"
#include "rapidjson/document.h"
class Component
{
protected:
	GameObject* gameObject = nullptr;
	int m_id = -1;
public:
	int m_sortingOrder = 1;

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();
	Component* GetComponent();
	void SetGameObject(GameObject* obj);
	GameObject* GetGameObject();
	virtual void OnCollisionEnter(GameObject* obj);
	virtual void OnCollisionExit(GameObject* obj);
	void SetID(int n);
	int GetID();
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

