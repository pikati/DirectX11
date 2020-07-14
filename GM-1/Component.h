#pragma once
#include "GameObject.h"

class Component
{
protected:
	GameObject* gameObject = nullptr;
public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
	Component* GetComponent();
	void SetGameObject(GameObject* obj);
	GameObject* GetGameObject();

	template <typename T>
	T* GetComponent()
	{
		if (typeid(*this) == typeid(T))
		{
			return *this;
		}
	}
};

