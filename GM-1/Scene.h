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
	static int m_renderNum;
	static int m_nowRenderNum;
	static std::string m_currentSceneName;

	GameObject* AddGameObject(int layer);
	std::string SetDefaultName(int i, std::string name, std::string firstName);
	void UpdateGameObject();
	void MoveTmpObject();
	void ChangeLayer();
	void ErasePossessedObject();
	void EditorDraw();
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

	void SetRenderNum(int num);
	void AddRenderNum();
	int GetRenderNum();
	static void ChangeScene();
	void LoadScene(std::string path);
	void SaveScene(std::string path);
	void ObjectInitialize();
	void DeleteObject(int layer, int index);
	std::string GetSceneName();
	void CreateScene();
	void PlayInitialize();
	void PlayFinalize();


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

