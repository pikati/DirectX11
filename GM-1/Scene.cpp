#define _CRT_SECURE_NO_WARNINGS
#include "Scene.h"
#include "SaveLoadManager.h"
#include "ClassDictionary.h"
#include "input.h"
#include <typeinfo>
#include <sstream>
#include "Constants.h"
#include "LevelLoader.h"
#include "Transform.h"

#include "GameObject.h"
#include "Plane.h"
#include "Billboard.h"
#include "SphereCollider.h"

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

std::list<GameObject*> Scene::m_gameObject[LAYER_MAX];
std::list<GameObject*> Scene::m_tempObject;
static std::list<GameObject*> tempObj;

enum AnimationName
{
	WALK,
	SLASH,
	STAB,
	SHOOT,
	DAMAGE,
	IDOL,
	END
};
Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Initialize()
{
	LevelLoader::LoadLevel(this, "save1.scene");
	GameObject* obj = CreateGameObject();
	SphereCollider* s = obj->AddComponent<SphereCollider>();
	s->SetCenter(0.0f, 0.0f, 0.0f);
	s->SetRadius(0.5f);
	s->Initialize();
	Plane* p = obj->AddComponent<Plane>();
	p->SetTextureName("Asset/Texture/Item/apple.png");
	p->Initialize();
	Billboard* b = obj->AddComponent<Billboard>();
	b->Initialize();
	//obj->transform->position.Set(-2.0f, 2.0f, 3.0f);
	obj->name = "Apple";
	obj->tag = "Item";
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

	for (GameObject* object : tempObj)
	{
		m_gameObject[object->layer].emplace_back(object);
		object->Update();
	}
	tempObj.clear();
	

	//todo::この機能に問題あるの草
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
	
	if (CInput::GetKeyTrigger('O'))
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* obj : m_gameObject[i])
			{
				obj->Destroy();
			}
		}
		LevelLoader::LoadLevel(this, "save1.scene");
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
		LevelLoader::SaveLevel(this, "save1.scene");
	}
	if (CInput::GetKeyTrigger('Q'))
	{
		Finalize();
		Initialize();
	}
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
		for (GameObject* object : tempObj)
		{
			object->Finalize();
			delete object;
		}
		tempObj.clear();
	}
	
}

GameObject* Scene::CreateGameObject()
{
	GameObject* obj = new GameObject();
	tempObj.emplace_back(obj);
	return obj;
}

void Scene::AddGameObject(GameObject* obj, bool isTemp)
{
	if (isTemp)
	{
		tempObj.emplace_back(obj);
	}
	else
	{
		m_gameObject[obj->layer].emplace_back(obj);
	}
}

GameObject* Scene::CreatePrefab(GameObject* obj)
{
	GameObject* object = new GameObject(*obj);
	tempObj.emplace_back(object);
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