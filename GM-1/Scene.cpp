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
#include "Editer.h"

std::list<GameObject*> Scene::m_gameObject[LAYER_MAX];
std::list<GameObject*> Scene::m_tempObject;
bool Scene::m_isChange = false;
int Scene::m_renderNum = 0;
int Scene::m_nowRenderNum = 0;

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Initialize()
{
	AudioManager::SetVolume(0);
	LevelLoader::LoadLevel(this, "Asset/Scene/stage1.scene");
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
		//���C���ԍ��ƊǗ����C�����قȂ����ꍇ�ɓ���ւ���
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

	//���X�g�Ɋ֐������s������ture�Ȃ烊�X�g����폜�����
	//[]������Ɩ��O�̂Ȃ��֐��ɂł���
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
	if (CInput::GetKeyTrigger('R'))
	{
		Finalize();
		ObjectPooler::Finalize();
		Editer::Finalize();
		LevelLoader::LoadLevel(this, "Asset/Scene/stage1.scene");
	}
}

void Scene::Draw()
{
	if (m_isChange)
	{
		m_isChange = false;
		return;
	}

	for (; m_nowRenderNum < m_renderNum; m_nowRenderNum++)
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObject[i])
			{
				object->Draw();
			}
		}
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