#pragma once
#include <vector>
#include <string>
#include "GameObject.h"
class ObjectPooler
{
private:
	std::vector<GameObject*> m_objects;
public:
	void SetObject(GameObject* obj);
	GameObject* GetObject(std::string name);

	template <typename T>
	GameObject* GetObject()
	{
		for (int i = 0; i < m_objects.size(); i++)
		{
			if (typeid(*m_objects[i]) == typeid(*T))
			{
				return m_objects[i];
			}
		}
	}
	return nullptr;
};

