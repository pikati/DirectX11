#define _CRT_SECURE_NO_WARNINGS
#include "Scene.h"
#include "SaveLoadManager.h"
#include "ClassDictionary.h"
#include "input.h"
#include "Collider.h"
#include <typeinfo>
#include <sstream>
#include "Constants.h"

#include <Stdio.h>
#ifdef _DEBUG
#   define MyOutputDebugString( str, ... ) \
      { \
        TCHAR c[256]; \
        sprintf( c, str, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }
#else
#    define MyOutputDebugString( str, ... ) // 空実装
#endif

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Initialize()
{
	*m_gameObject = SaveLoadManager::Load(m_gameObject);
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* obj : m_gameObject[i])
		{
			obj->Initialize();
		}
	}
	
}

void Scene::Update()
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : m_gameObject[i])
		{
			object->Update();
		}
	}

	for (int i = 0; i < LAYER_MAX; i++)
	{
		//レイヤ番号と管理レイヤが異なった場合に入れ替える
		auto itr = m_gameObject[i].begin();
		for (GameObject* object : m_gameObject[i])
		{
			if (object->layer != i)
			{
				m_gameObject[i].erase(itr);
				m_gameObject[object->layer].push_back(object);
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
	

	if (CInput::GetKeyTrigger('X'))
	{
		SaveLoadManager::Save(m_gameObject);
	}

	Collider::UpdateCollision();
}

void Scene::Draw()
{
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
	}
	
}

GameObject* Scene::CreateGameObject()
{
	GameObject* obj = new GameObject();
	m_gameObject[obj->layer].push_back(obj);
	return obj;
}

GameObject* Scene::CreatePrefab(GameObject* obj)
{
	GameObject* object = new GameObject(*obj);
	m_gameObject[obj->layer].push_back(object);
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