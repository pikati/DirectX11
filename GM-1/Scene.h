#pragma once
#include <list>
#include "main.h"
#include "GameObject.h"
#include "Constants.h"
#include <string>
#include <typeinfo>

class Scene
{
private:
	static std::list<GameObject*> m_gameObject[LAYER_MAX];
	static std::list<GameObject*> m_tempObject;
	static bool m_isChange;

	GameObject* AddGameObject();
public:
	Scene();
	~Scene();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	GameObject* CreateGameObject();
	void AddGameObject(GameObject* obj, bool isTemp = true);
	GameObject* CreatePrefab(GameObject* obj);
	GameObject* LoadPrefab(std::string path);
	GameObject* Find(std::string name);
	std::list<GameObject*>* GetAllGameObject();

	static void ChangeScene();

	template <typename T>
	GameObject* Find()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObject[i])
			{
				for (Component* component : object->GetComponents())
				{
					if (typeid(*component) == typeid(T))
					{
						return object;
					}
				}
			}
		}
		return nullptr;
	}
};

