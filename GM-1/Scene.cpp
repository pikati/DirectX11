#define _CRT_SECURE_NO_WARNINGS
#include "Scene.h"
#include "SaveLoadManager.h"
#include "ClassDictionary.h"
#include "input.h"
#include "Collider.h"
#include <typeinfo>
#include <sstream>
#include "Constants.h"
#include "LevelLoader.h"
#include "Field.h"
#include "Texture.h"
#include "Transform.h"
#include "Fbx.h"
#include "Animation.h"

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

static Fbx* f;
static Animation* anim;
static int state;
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
	/*GameObject* obj = CreateGameObject();
	anim = obj->AddComponent<Animation>();
	anim->SetAnimationNum(END);
	anim->SetAnimationData(WALK, true, 0, 62, 20, 60);
	anim->SetAnimationData(SLASH, false, 63, 94);
	anim->SetAnimationData(STAB, false, 95, 135);
	anim->SetAnimationData(SHOOT, false, 136, 171);
	anim->SetAnimationData(DAMAGE, false, 172, 212);
	anim->SetAnimationData(IDOL, false, 0, 0);
	state = WALK;
	anim->SetState(state);
	f = obj->AddComponent<Fbx>();
	f->Initialize();
	f->Load("Asset/Models/Player/eatmanV3.fbx");*/
	
}

void Scene::Update()
{
	/*if (CInput::GetKeyTrigger('T'))
	{
		f->UpFrame();
	}
	if (CInput::GetKeyTrigger('G'))
	{
		f->DownFrame();
	}
	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		f->PlayAnimation();
	}
	if (CInput::GetKeyTrigger('R'))
	{
		state++;
		if (state >= END)
		{
			state = WALK;
		}
		anim->SetState(state);
	}
	if (CInput::GetKeyTrigger('F'))
	{
		state--;
		if (state < WALK)
		{
			state = IDOL;
		}
		anim->SetState(state);
	}*/
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : m_gameObject[i])
		{
			object->Update();
		}
	}


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
				m_gameObject[object->layer].push_back(object);
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

void Scene::AddGameObject(GameObject* obj)
{
	m_gameObject[obj->layer].push_back(obj);
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

std::list<GameObject*>* Scene::GetAllGameObject()
{
	return m_gameObject;
}