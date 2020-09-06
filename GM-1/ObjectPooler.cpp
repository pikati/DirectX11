#include "ObjectPooler.h"
#include "LevelLoader.h"
#include "Scene.h"
#include "manager.h"

std::vector<Texture*> ObjectPooler::m_texture;
std::vector<std::string> ObjectPooler::m_textureName;
std::vector<Fbx*> ObjectPooler::m_fbx;
std::vector<GameObject*> ObjectPooler::m_object;

GameObject* ObjectPooler::SetGameObject(GameObject* obj)
{
	m_object.emplace_back(obj);
	return obj;
}

GameObject* ObjectPooler::LoadGameObject(std::string fileName)
{
	GameObject* obj = LevelLoader::LoadPrefab(fileName.c_str());
	m_object.emplace_back(obj);
	return obj;
}

GameObject* ObjectPooler::CreatePrefab(std::string name)
{
	for (GameObject* obj : m_object)
	{
		if (name == obj->name)
		{
			GameObject* ob = new GameObject(*obj);
			ob->Initialize();
			CManager::GetScene()->AddGameObject(ob);
			return ob;
		}
	}
	return nullptr;
}

void ObjectPooler::SetComponent(Texture* tex, std::string textureName)
{
	m_texture.emplace_back(tex);
	m_textureName.emplace_back(textureName);
}

void ObjectPooler::SetComponent(Fbx* fbx)
{
	m_fbx.emplace_back(fbx);
}

Texture* ObjectPooler::GetTexture(int id)
{
	if (id < m_texture.size())
	{
		return m_texture[id];
	}
	return nullptr;
}

Texture* ObjectPooler::GetTexture(std::string name)
{
	for (int i = 0; i < m_textureName.size(); i++)
	{
		if (name == m_textureName[i])
		{
			return m_texture[i];
		}
	}
	return nullptr;
}

Fbx* ObjectPooler::GetComponent(int id, Fbx* tex)
{
	if (id < m_fbx.size())
	{
		return m_fbx[id];
	}
	return nullptr;
}

void ObjectPooler::Clear()
{
	for (int i = m_texture.size() - 1; i >= 0; i--)
	{
		m_texture[i]->Finalize();
	}
	m_texture.clear();
	for (int i = m_texture.size() - 1; i >= 0; i--)
	{
		delete m_texture[i];
	}


	for (int i = m_fbx.size() - 1; i >= 0; i--)
	{
		m_fbx[i]->Finalize();
	}
	m_fbx.clear();
	for (int i = m_fbx.size() - 1; i >= 0; i--)
	{
		delete m_fbx[i];
	}


	for (int i = m_object.size() - 1; i >= 0; i--)
	{
		m_object[i]->Finalize();
	}
	m_object.clear();
	for (int i = m_object.size() - 1; i >= 0; i--)
	{
		delete m_object[i];
	}
	m_textureName.clear();
}