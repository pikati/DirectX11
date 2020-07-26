#pragma once
#include <vector>
#include <string>
#include "Texture.h"
#include <memory>
class ObjectPooler
{
private:
	static std::vector<Texture*> m_texture;

public:
	template <typename T>
	static T* SetComponent(T* component)
	{
		std::string a = typeid(T).name();
		std::string b = typeid(Texture).name();
		if (a == b)
		{
			m_texture.push_back(component);
		}
		return component;
	}

	template <typename T>
	static T* GetComponent(int id)
	{
		if (id == -1) return nullptr;
		int i = m_texture.size() - 1;
		if (i < id) return nullptr;
		
		std::string a = typeid(T).name();
		std::string b = typeid(Texture).name();
		if (a == b)
		{
			
			return m_texture[id];
		}
		return nullptr;
	}
};

