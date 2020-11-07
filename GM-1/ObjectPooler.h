#pragma once
#include <vector>
#include <string>
#include "Texture.h"
#include "Fbx.h"
#include "GameObject.h"
#include <memory>
class ObjectPooler
{
private:
	static std::vector<Texture*> m_texture;
	static std::vector<std::string> m_textureName;
	static std::vector<Fbx*> m_fbx;
	static std::vector<GameObject*> m_object;
public:
	static void Initialize();
	static void Finalize();
	static GameObject* SetGameObject(GameObject* obj);
	static GameObject* LoadGameObject(std::string fileName);
	static GameObject* CreatePrefab(std::string name);
	static void SetComponent(Texture* tex, std::string textureName);
	static void SetComponent(Fbx* fbx);
	static Texture* GetTexture(int id);
	static Texture* GetTexture(std::string name);
	static Fbx* GetComponent(int id, Fbx* fbx);
	static void Clear();

	//template <typename T>
	//static T* SetComponent(T* component, std::string textureName)
	//{
	//	std::string a = typeid(T).name();
	//	std::string b = typeid(Texture).name();
	//	//std::string c = typeid(Fbx).name();
	//	if (a == b)
	//	{
	//		m_texture.emplace_back(component);
	//		m_textureName.emplace_back(textureName);
	//	}
	//	/*if (a == c)
	//	{
	//		m_fbx.emplace_back(component);
	//	}*/
	//	return component;
	//}

	//template <typename T>
	//static T* GetComponent(int id)
	//{
	//	if (id == -1) return nullptr;
	//	int i = m_texture.size() - 1;
	//	if (i < id) return nullptr;
	//	
	//	std::string a = typeid(T).name();
	//	std::string b = typeid(Texture).name();
	//	//std::string c = typeid(Fbx).name();
	//	if (a == b)
	//	{
	//		return m_texture[id];
	//	}
	//	/*if (a == c)
	//	{
	//		return m_fbx[id];
	//	}*/
	//	return nullptr;
	//}
};

