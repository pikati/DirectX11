#define _CRT_SECURE_NO_WARNINGS
#include "Scene.h"
#include "SaveLoadManager.h"
#include "input.h"
#include "Constants.h"
#include "LevelLoader.h"
#include "Collider.h"
#include "SceneManager.h"
#include "AudioManager.h"

#include "BoxCollider.h"
#include "Transform.h"

std::list<GameObject*> Scene::m_gameObject[LAYER_MAX];
std::list<GameObject*> Scene::m_tempObject;
bool Scene::m_isChange = false;

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Initialize()
{
	AudioManager::SetVolume(0);
	LevelLoader::LoadLevel(this, "Asset/Scene/OBBTest.scene");
	GameObject* obj = new GameObject();
	AddGameObject(obj, false);
	obj->AddComponent<BoxCollider>();
	obj->Initialize();
	//obj->transform->rotation.Set(0, 20.0f, 0);
}

void Scene::Update()
{
	Collider::UpdateCollision();

	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : m_gameObject[i])
		{
			object->Update();
		}
	}

	for (GameObject* object : m_tempObject)
	{
		m_gameObject[object->layer].emplace_back(object);
		object->Update();
	}
	m_tempObject.clear();
	

	for (int i = 0; i < LAYER_MAX; i++)
	{
		//レイヤ番号と管理レイヤが異なった場合に入れ替える
		auto itr = m_gameObject[i].begin();
		for (GameObject* object : m_gameObject[i])
		{
			if (object->layer != i)
			{
				m_gameObject[i].erase(itr);
				m_gameObject[object->layer].emplace_back(object);
				itr--;
				itr = m_gameObject[i].begin();
			}
			itr++;
		}
	}

	//リストに関数を実行させてtureならリストから削除される
	//[]をつけると名前のない関数にできる
	for (int i = 0; i < LAYER_MAX; i++)
	{
		m_gameObject[i].remove_if([](GameObject* object)
			{
				if (object->isDestroy)
				{
					object->Finalize();
					delete object;
					return true;
				}
				return false;
			}
		);
	}

	if (m_isChange)
	{
 		Finalize();
		SceneManager::LoadScene();
	}
}

void Scene::Draw()
{
	if (m_isChange)
	{
		m_isChange = false;
		return;
	}

	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : m_gameObject[i])
		{
			object->Draw();
		}
	}
}


void Scene::Finalize()
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : m_gameObject[i])
		{
			object->Finalize();
			delete object;
		}
		m_gameObject[i].clear();
		for (GameObject* object : m_tempObject)
		{
			object->Finalize();
			delete object;
		}
		m_tempObject.clear();
	}
	
}

GameObject* Scene::CreateGameObject()
{
	GameObject* obj = new GameObject();
	m_tempObject.emplace_back(obj);
	return obj;
}

void Scene::AddGameObject(GameObject* obj, bool isTemp)
{
	if (isTemp)
	{
		m_tempObject.emplace_back(obj);
	}
	else
	{
		m_gameObject[obj->layer].emplace_back(obj);
	}
}

GameObject* Scene::CreatePrefab(GameObject* obj)
{
	GameObject* object = new GameObject(*obj);
	m_tempObject.emplace_back(object);
	object->Initialize();
	return object;
}

GameObject* Scene::LoadPrefab(std::string path)
{
	return SaveLoadManager::Instantiate(path);
}

GameObject* Scene::Find(std::string name)
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : m_gameObject[i])
		{
			if (object->name == name)
			{
				return object;
			}
		}
	}
	
	return nullptr;
}

std::list<GameObject*>* Scene::GetAllGameObject()
{
	return m_gameObject;
}

void Scene::ChangeScene()
{
	m_isChange = true;
}

GameObject* Scene::AddGameObject()
{
	GameObject* obj = new GameObject();
	m_gameObject[obj->layer].emplace_back(obj);
	return obj;
}