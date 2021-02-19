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
#include "DirectionalLight.h"
#include "Camera.h"
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
	m_currentSceneName = "Asset/Scene/title.scene";
	LevelLoader::LoadLevel(this, m_currentSceneName.c_str());
	Hierarchy::SetDefaultPath(m_currentSceneName);
}

void Scene::Update()
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : m_gameObject[i])
		{
			object->FixedUpdate();
		}
	}

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

	if (!Editor::IsPlay())
	{
		EditorDraw();
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
		/*std::string str;
		str = std::to_string(m_nowRenderNum) + "\n";
		OutputDebugString(str.c_str());*/
	}
	m_nowRenderNum = 0;
}

void Scene::Finalize()
{
	PlayFinalize();
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
	Collider::FinalizeCollider();
}

GameObject* Scene::CreateGameObject()
{
	GameObject* obj = new GameObject();
	obj->name = SetDefaultName(0, "unname", "unname");
	m_tempObject.emplace_back(obj);
	obj->Initialize();
	return obj;
}

void Scene::AddGameObject(GameObject* obj, bool isTemp)
{
	obj->name  = SetDefaultName(0, obj->name, obj->name);
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

GameObject* Scene::AddGameObject(int layer)
{
	GameObject* obj = new GameObject();
	obj->layer = layer;
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

std::string Scene::SetDefaultName(int i, std::string name, std::string firstName)
{
	std::string newName = firstName;
	if (i != 0)
	{
		std::string num = std::to_string(i);
		newName += num;
	}
	for (int j = 0; j < LAYER_MAX; j++)
	{
		for (GameObject* obj : m_gameObject[j])
		{
			if (i == 0)
			{
				if (obj->name == firstName)
				{
					name = SetDefaultName(1, newName, firstName);
				}
			}
			else
			{
				
				if (obj->name == name)
				{
					name = SetDefaultName(i + 1, newName, firstName);
				}
			}
		}
	}
	return name;
}

void Scene::LoadScene(std::string path)
{
	if (!LevelLoader::ExistSceneFile(path))
	{
		MessageBox(NULL, "failed to load level", NULL, MB_OK);
		return;
	}
	Finalize();
	ObjectPooler::Finalize();
	Editor::Finalize();
	Collider::FinalizeCollider();
	LevelLoader::LoadLevel(this, path.c_str());
	m_currentSceneName = path;
	Hierarchy::SetDefaultPath(m_currentSceneName);
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
			obj->SystemInitialize();
			if (Editor::IsPlay)
			{
				obj->Initialize();
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
					object->SystemFinalize();
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

void Scene::CreateScene()
{
	Finalize();
	ObjectPooler::Finalize();
	Editor::Finalize();
	m_currentSceneName = "Asset/Scene/sampleScene.scene";
	GameObject* obj2 = AddGameObject(0);
	Camera* c = obj2->AddComponent<Camera>();
	D3D11_VIEWPORT v;
	v.Width = SCREEN_WIDTH;
	v.Height = SCREEN_HEIGHT;
	v.TopLeftX = 0;
	v.TopLeftY = 0;
	c->SetViwePort(v);
	c->SetLookAt(Vector3(0, 0, 0));
	obj2->name = "MainCamera";
	obj2->tag = "MainCamera";
	obj2->transform->position.Set(0, 10.0f, -10.0f);
	obj2->SystemInitialize();
	GameObject* obj = AddGameObject(0);
	obj->AddComponent<DirectionalLight>();
	obj->name = "DirectionalLight";
	obj->tag = "Light";
	obj->transform->position.Set(10.0f, 10.0f, 10.0f);
	obj->SystemInitialize();
}

void Scene::PlayInitialize()
{
	for (int j = 0; j < LAYER_MAX; j++)
	{
		for (GameObject* obj : m_gameObject[j])
		{
			obj->Initialize();
		}
	}
}

void Scene::PlayFinalize()
{
	for (int j = 0; j < LAYER_MAX; j++)
	{
		for (GameObject* obj : m_gameObject[j])
		{
			obj->Finalize();
		}
	}
}

void Scene::EditorDraw()
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : m_gameObject[i])
		{
			object->Draw();
		}
	}
}