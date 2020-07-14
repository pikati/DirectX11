#pragma once
#include <list>
#include "main.h"
#include "GameObject.h"
#include "Constants.h"
#include <string>
#include <typeinfo>

class Scene
{
protected:
	std::list<GameObject*> m_gameObject[LAYER_MAX];
public:
	Scene();
	virtual ~Scene();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();

	GameObject* CreateGameObject();
	GameObject* CreatePrefab(GameObject* obj);
	GameObject* LoadPrefab(std::string path);
	GameObject* Find(std::string name);

	template <typename T>
	T* Find()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}
};

