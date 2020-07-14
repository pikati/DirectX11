#include "Component.h"



Component* Component::GetComponent()
{
	//Component* c = new decltype(*this)();
	return this;
}

void Component::SetGameObject(GameObject* obj)
{
	gameObject = obj;
}

GameObject* Component::GetGameObject()
{
	return gameObject;
}