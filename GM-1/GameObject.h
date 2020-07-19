#pragma once
#include "Engine.h"
#include <string>
#include <list>
#include <typeinfo>
#include "rapidjson/document.h"

class Component;
class Transform;

class GameObject
{
private:
	std::list<Component*> components;

	bool DeleteComponent(Component* component);
protected:
public:
	GameObject(const GameObject& obj);
	std::string name = "unname";
	std::string tag = "untagged";
	bool activeSelf = true;
	bool isDestroy = false;
	Transform* transform;
	int layer = 1;

	GameObject();
	~GameObject();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Destroy();

	Vector3 GetForward();

	void LoadProperties(const rapidjson::Value& inObj);

	std::list<Component*> GetComponents() const;
	void SetComponents(std::list<Component*> component);










	template <typename T>
	T* AddComponent()
	{
		T* component = new T();
		components.push_back(component);
		component->SetGameObject(this);
		component->Initialize();
		return component;
	}

	Component* AddComponent(Component* component);

	template <typename T>
	T* GetComponent()
	{
		T* component = new T();
		for (Component* c : components)
		{
			if (typeid(*c) == typeid(T))
			{
				component = (T*)c;
			}
		}
		return component;
	}

	Component* GetComponent(std::string name);

	template<typename T>
	void DeleteComponent()
	{
		for (Component* component : components)
		{
			if (typeid(*component) == typeid(T))
			{
				components.remove_if(DeleteComponent(component));
			}
		}
	}
};