#include "ObjectPooler.h"

void ObjectPooler::SetObject(GameObject* obj)
{
	m_objects.push_back(obj);
}

GameObject* ObjectPooler::GetObject(std::string name)
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		if (m_objects[i]->name == name)
		{
			return m_objects[i];
		}
	}
	return nullptr;
}