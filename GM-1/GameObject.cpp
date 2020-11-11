#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "ClassDictionary.h"
#include <sstream>
#include "main.h"
#include <DirectXMath.h>
#include "LevelLoader.h"

GameObject::GameObject()
{
	transform = new Transform();
}

GameObject::GameObject(const GameObject& obj)
{
	transform = new Transform();
	std::list<Component*> c2;
	this->name = obj.name;
	this->tag = obj.tag;
	this->activeSelf = obj.activeSelf;
	this->isDestroy = obj.isDestroy;
	this->layer = obj.layer;
	for (Component* component : obj.components)
	{
		std::string typeName = typeid(*component).name();
		std::stringstream ss;
		ss << typeName;
		std::string getStr;
		while (!ss.eof())
		{
			ss >> getStr;
		}
		Component* c = ClassDictionary::SetComponent(getStr);
		c->SetProperties(component);
		c2.push_back(c);
		c->SetGameObject(this);
		c->SetID(component->GetID());
	}
	this->components = c2;
}

GameObject::~GameObject()
{

}

void GameObject::Initialize()
{
	transform->Initialize();
	transform->SetGameObject(this);
	for (Component* c : components)
	{
		c->Initialize();
		c->SetGameObject(this);
	}
}

void GameObject::Update()
{
	if (!activeSelf)
	{
		return;
	}
	transform->Update();
	for (Component* c : components)
	{
		c->Update();
	}
}

void GameObject::Draw()
{
	if (!activeSelf)
	{
		return;
	}

	transform->Draw();
	for (Component* c : components)
	{
		c->Draw();
	}
}

void GameObject::Finalize()
{
	transform->Finalize();
	delete transform;
	for (Component* c : components)
	{
		c->Finalize();
	}
}

void GameObject::OnCollisionEnter(GameObject* obj)
{
	for (Component* c : components)
	{
		c->OnCollisionEnter(obj);
	}
}

void GameObject::OnCollisionExit(GameObject* obj)
{
	for (Component* c : components)
	{
		c->OnCollisionExit(obj);
	}
}

Vector3 GameObject::GetForward()
{
	using namespace DirectX;
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, XMConvertToRadians(transform->rotation.y), XMConvertToRadians(transform->rotation.x), XMConvertToRadians(transform->rotation.z));
	Vector3 forward;
	forward.x = rot._31;
	forward.y = rot._32;
	forward.z = rot._33;
	return forward;
}

Component* GameObject::AddComponent(Component* component)
{
	components.push_back(component);
	component->SetGameObject(this);
	return component;
}

Component* GameObject::GetComponent(std::string name)
{
	name = "class " + name;
	for (Component* c : components)
	{
		if (typeid(*c).name() == name)
		{
			return c;
		}
	}
	return nullptr;
}

void GameObject::Destroy()
{
	isDestroy = true;
}

std::list<Component*> GameObject::GetComponents() const
{
	return components;
}

void GameObject::SetComponents(std::list<Component*> components)
{
	this->components = components;
}

bool GameObject::DeleteComponent(Component* component)
{
	component->Finalize();
	delete component;
	return true;
}