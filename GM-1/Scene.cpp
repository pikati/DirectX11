#define _CRT_SECURE_NO_WARNINGS
#include "Scene.h"
#include "SaveLoadManager.h"
#include "input.h"
#include "Constants.h"
#include "LevelLoader.h"
#include "Collider.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "ObjectPooler.h"
#include "Editor.h"
#include "Hierarchy.h"
#include <algorithm>

#include "Fbx.h"
#include "GameObject.h"
#include "Transform.h"

std::list<GameObject*> Scene::m_gameObject[LAYER_MAX];
std::list<GameObject*> Scene::m_tempObject;
bool Scene::m_isChange = false;
int Scene::m_renderNum = 0;
int Scene::m_nowRenderNum = 0;
std::string Scene::m_currentSceneName;

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Initialize()
{
	AudioManager::SetVolume(0);
	m_currentSceneName = "Asset/Scene/stage1.scene";
	LevelLoader::LoadLevel(this, m_currentSceneName.c_str());
	Hierarchy::SetDefaultPath(m_currentSceneName);
	
	/*GameObject* obj = CreateGameObject();
	Fbx* f = obj->AddComponent<Fbx>();
	f->SetFileName("Asset/Models/Player/Cat.fbx");
	f->SetTextureName("Asset/Texture/Player/Cat.png");
	obj->Initialize();
	obj->transform->position.y = 1.5f;*/
}

void Scene::Update()
{
	UpdateGameObject();

	MoveTmpObject();

	ChangeLayer();

	ErasePossessedObject();

	if (m_isChange)
	{
 		Finalize();
		ObjectPooler::Finalize();
		Editor::Finalize();
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

	std::list<GameObject*> obj;

	for (; m_nowRenderNum < m_renderNum; m_nowRenderNum++)
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObject[i])
			{
				object->Draw();
				//obj.emplace_back(object);
			}
		}
		/*std::sort(obj.begin(), obj.end());
		for (GameObject* o : obj)
		{
			o->Draw();
		}
		obj.clear();*/
	}
	m_nowRenderNum = 0;
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
	obj->name = SetDefaultName(0);
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

void Scene::SetRenderNum(int num)
{
	m_renderNum = num;
}

void Scene::AddRenderNum()
{
	m_renderNum++;
}

int Scene::GetRenderNum()
{
	return m_nowRenderNum;
}

std::string Scene::SetDefaultName(int i)
{
	std::string name = "unname";
	if (i != 0)
	{
		std::string num = std::to_string(i);
		name += num;
	}
	for (int j = 0; j < LAYER_MAX; j++)
	{
		for (GameObject* obj : m_gameObject[j])
		{
			if (i == 0)
			{
				if (obj->name == "unname")
				{
					name = SetDefaultName(1);
				}
			}
			else
			{
				
				if (obj->name == name)
				{
					name = SetDefaultName(i + 1);
				}
			}
		}
	}
	return name;
}

void Scene::LoadScene(std::string path)
{
	Finalize();
	ObjectPooler::Finalize();
	Editor::Finalize();
	LevelLoader::LoadLevel(this, path.c_str());
}

void Scene::SaveScene(std::string path)
{
	LevelLoader::SaveLevel(this, path.c_str());
}

void Scene::ObjectInitialize()
{
	for (int j = 0; j < LAYER_MAX; j++)
	{
		for (GameObject* obj : m_gameObject[j])
		{
			for (Component* c : obj->GetComponents())
			{
				c->Initialize();
			}
		}
	}
}

void Scene::DeleteObject(int layer, int index)
{
	int count = 0;
	for (GameObject* obj : m_gameObject[layer])
	{
		if (count == index)
		{
			obj->Destroy();
			return;
		}
		count++;
	}
}

void Scene::UpdateGameObject()
{
	if (Editor::IsPlay())
	{
		Collider::UpdateCollision();

		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObject[i])
			{
				object->Update();
			}
		}
	}

	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : m_gameObject[i])
		{
			object->SystemUpdate();
		}
	}
}

void Scene::MoveTmpObject()
{
	for (GameObject* object : m_tempObject)
	{
		m_gameObject[object->layer].emplace_back(object);
		object->Update();
	}
	m_tempObject.clear();
}

void Scene::ChangeLayer()
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		//ƒŒƒCƒ„”Ô†‚ÆŠÇ—ƒŒƒCƒ„‚ªˆÙ‚È‚Á‚½ê‡‚É“ü‚ê‘Ö‚¦‚é
		m_gameObject[i].remove_if([i](GameObject* object)
			{
				if (object->layer != i)
				{
					m_gameObject[object->layer].emplace_back(object);
					return true;
				}
				return false;
			});
	}
}

void Scene::ErasePossessedObject()
{
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
}

std::string Scene::GetSceneName()
{
	return m_currentSceneName;
}