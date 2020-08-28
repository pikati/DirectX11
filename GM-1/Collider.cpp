#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "AABB.h"
#include <math.h>
#include "ImguiManager.h"

std::vector<Collider*> Collider::m_colliders;

bool Collider::IsCollision()
{
	return m_isCollision;
}

int Collider::SetCollider(Collider* collider)
{
	m_colliders.push_back(collider);
	collider->m_hitObject.resize(8);
	collider->m_exitObject.resize(8);
	return m_colliders.size() - 1;
}

void Collider::DeleteCollider(int colliderID)
{
	m_colliders.erase(m_colliders.begin() + colliderID);
	for (int i = colliderID; i < m_colliders.size(); i++)
	{
		m_colliders[i]->m_colliderID--;
	}
}

void Collider::UpdateCollision()
{
	for (int i = 0; i < m_colliders.size(); i++)
	{
		m_colliders[i]->Update();
	}
	int a = m_colliders.size() - 1;
	int b = a + 1;
	for (int i = 0; i < a; i++)
	{
		for (int j = i + 1; j < b; j++)
		{
			RunCollisionDetection(m_colliders[i], m_colliders[j]);
		}
	}
	for (int i = 0; i < m_colliders.size(); i++)
	{
		if (!m_colliders[i]->m_isCollisionThisFrame)
		{
			m_colliders[i]->m_exitObject = m_colliders[i]->m_hitObject;
			for (int j = 0; j < m_colliders[i]->m_hitObject.size(); j++)
			{
				m_colliders[i]->m_hitObject[j] = nullptr;
			}
		}
	}
}

void Collider::RunCollisionDetection(Collider* c1, Collider* c2)
{
	if (c1->m_colliderType == Sphere && c2->m_colliderType == Sphere)
	{
		Sphere2Sphere(c1, c2);
	}
	else if (c1->m_colliderType == Sphere && c2->m_colliderType == Mesh)
	{
		Sphere2Mesh(c1, c2);
	}
	else if (c1->m_colliderType == Mesh && c2->m_colliderType == Sphere)
	{
		Sphere2Mesh(c2, c1);
	}
	else if (c1->m_colliderType == Aabb && c2->m_colliderType == Aabb)
	{
		AABB2AABB(c1, c2);
	}
	else if (c1->m_colliderType == Sphere && c2->m_colliderType == Aabb)
	{
		Sphere2AABB(c1, c2);
	}
	else if (c1->m_colliderType == Aabb && c2->m_colliderType == Sphere)
	{
		Sphere2AABB(c2, c1);
	}
}

void Collider::Destroy()
{
	m_isDestroy = true;
}

float Collider::GetRadius()
{
	return 0;
}

std::vector<GameObject*> Collider::GetHitGameObject()
{
	return m_hitObject;
}

std::vector<GameObject*> Collider::GetExitGameObject()
{
	return m_exitObject;
}

void Collider::ResetExitCollision()
{
	for (int i = 0; i < m_colliders.size(); i++)
	{
		for (int j = 0; j < m_colliders[i]->m_exitObject.size(); j++)
		{
			m_colliders[i]->m_exitObject[j] = m_colliders[i]->m_hitObject[j];
			m_colliders[i]->m_hitObject[j] = nullptr;
		}
	}
}

void Collider::Sphere2Sphere(Collider* c1, Collider* c2)
{
	Vector3 c1Position = c1->GetPosition();
	float c1Radius = c1->GetRadius();
	Vector3 c2Position = c2->GetPosition();
	float c2Radius = c2->GetRadius();

	if (powf(c2Position.x - c1Position.x, 2.0f) + powf(c2Position.y - c1Position.y, 2.0f) + powf(c2Position.z - c1Position.z, 2.0f) <= powf(c1Radius + c2Radius, 2.0f))
	{
		c1->m_isCollision = true;
		c2->m_isCollision = true;
		c1->m_isCollisionThisFrame = true;
		c2->m_isCollisionThisFrame = true;
		for (int i = 0; i < c1->m_hitObject.size(); i++)
		{
			if (c1->m_hitObject[i] == nullptr)
			{
				c1->m_hitObject[i] = c2->GetGameObject();
			}
		}
		for (int i = 0; i < c2->m_hitObject.size(); i++)
		{
			if (c2->m_hitObject[i] == nullptr)
			{
				c2->m_hitObject[i] = c1->GetGameObject();
			}
		}
	}
	else
	{
		if (!c1->m_isCollisionThisFrame)
		{
			c1->m_isCollision = false;
		}
		if (!c2->m_isCollisionThisFrame)
		{
			c2->m_isCollision = false;
		}
	}
}

void Collider::Sphere2Mesh(Collider* c1, Collider* c2)
{
	Vector3 spherePos[6];
	for (int i = 0; i < 6; i++)
	{
		spherePos[i] = c1->GetPosition();
	}
	float rad = c1->GetRadius();
	spherePos[0] += Vector3(rad, 0, 0);
	spherePos[1] += Vector3(-rad, 0, 0);
	spherePos[2] += Vector3(0, rad, 0);
	spherePos[3] += Vector3(0, -rad, 0);
	spherePos[4] += Vector3(0, 0, rad);
	spherePos[5] += Vector3(0, 0, -rad);

}

void Collider::AABB2AABB(Collider* c1, Collider* c2)
{
	AABB* a1 = dynamic_cast<AABB*>(c1);
	AABB* a2= dynamic_cast<AABB*>(c2);

	Vector3 min1 = a1->GetMin() + a1->GetPosition();
	Vector3 min2 = a2->GetMin() + a2->GetPosition();
	Vector3 max1 = a1->GetMax() + a1->GetPosition();
	Vector3 max2 = a2->GetMax() + a2->GetPosition();
	if (min1.x > max2.x)
	{
		if (!c1->m_isCollisionThisFrame)
		{
			c1->m_isCollision = false;
		}
		if (!c2->m_isCollisionThisFrame)
		{
			c2->m_isCollision = false;
		}
		return;
	}

	if (max1.x < min2.x)
	{
		if (!c1->m_isCollisionThisFrame)
		{
			c1->m_isCollision = false;
		}
		if (!c2->m_isCollisionThisFrame)
		{
			c2->m_isCollision = false;
		}
		return;
	}

	if (min1.y > max2.y)
	{
		if (!c1->m_isCollisionThisFrame)
		{
			c1->m_isCollision = false;
		}
		if (!c2->m_isCollisionThisFrame)
		{
			c2->m_isCollision = false;
		}
		return;
	}

	if (max1.y < min2.y)
	{
		if (!c1->m_isCollisionThisFrame)
		{
			c1->m_isCollision = false;
		}
		if (!c2->m_isCollisionThisFrame)
		{
			c2->m_isCollision = false;
		}
		return;
	}

	if (min1.z > max2.z)
	{
		if (!c1->m_isCollisionThisFrame)
		{
			c1->m_isCollision = false;
		}
		if (!c2->m_isCollisionThisFrame)
		{
			c2->m_isCollision = false;
		}
		return;
	}

	if (max1.z < min2.z)
	{
		if (!c1->m_isCollisionThisFrame)
		{
			c1->m_isCollision = false;
		}
		if (!c2->m_isCollisionThisFrame)
		{
			c2->m_isCollision = false;
		}
		return;
	}

	c1->m_isCollision = true;
	c2->m_isCollision = true;
	c1->m_isCollisionThisFrame = true;
	c2->m_isCollisionThisFrame = true;
	for (int i = 0; i < c1->m_hitObject.size(); i++)
	{
		if (c1->m_hitObject[i] == nullptr)
		{
			c1->m_hitObject[i] = c2->GetGameObject();
			break;
		}
	}
	for (int i = 0; i < c2->m_hitObject.size(); i++)
	{
		if (c2->m_hitObject[i] == nullptr)
		{
			c2->m_hitObject[i] = c1->GetGameObject();
			
		}
	}

	bool a1Kinematic = a1->IsKinematic();
	bool a2Kinematic = a2->IsKinematic();
	if (a1Kinematic && a2Kinematic) return;
	
	Vector3 pos1 = c1->gameObject->transform->position;
	Vector3 pos2 = c2->gameObject->transform->position;

	Vector3 max12 = a1->GetMax() + a1->GetPosition();
	Vector3 min12 = a1->GetMin() + a1->GetPosition();
	Vector3 max22 = a2->GetMax() + a2->GetPosition();
	Vector3 min22 = a2->GetMin() + a2->GetPosition();

	float dx1 = min22.x - max12.x;
	float dx2 = max22.x - min12.x;
	float dy1 = min22.y - max12.y;
	float dy2 = max22.y - min12.y;
	float dz1 = min22.z - max12.z;
	float dz2 = max22.z - min12.z;

	float dx = (fabsf(dx1) < fabsf(dx2)) ? dx1 : dx2;
	float dy = (fabsf(dy1) < fabsf(dy2)) ? dy1 : dy2;
	float dz = (fabsf(dz1) < fabsf(dz2)) ? dz1 : dz2;

	if (fabsf(dx) <= fabsf(dy) && fabsf(dx) <= fabsf(dz))
	{
		if (a1Kinematic)
		{
			pos2.x -= dx;
		}
		else if (a2Kinematic)
		{
			pos1.x += dx;
		}
		else if(!a1Kinematic && !a2Kinematic)
		{
			pos1.x += dx / 2.0f;
			pos2.x -= dx / 2.0f;
		}
	}
	else if (fabsf(dy) <= fabsf(dx) && fabsf(dy) <= fabsf(dz))
	{
		if (a1Kinematic)
		{
			pos2.y -= dy;
		}
		else if (a2Kinematic)
		{
			pos1.y += dy;
		}
		else if (!a1Kinematic && !a2Kinematic)
		{
			pos1.y += dy / 2.0f;
			pos2.y -= dy / 2.0f;
		}
	}
	else
	{
		if (a1Kinematic)
		{
			pos2.z -= dz;
		}
		else if (a2Kinematic)
		{
			pos1.z += dz;
		}
		else if (!a1Kinematic && !a2Kinematic)
		{
			pos1.z += dz / 2.0f;
			pos2.z -= dz / 2.0f;
		}
	}
	c1->gameObject->transform->position = pos1;
	c2->gameObject->transform->position = pos2;
}

void Collider::Sphere2AABB(Collider* c1, Collider* c2)
{
	SphereCollider* sc = dynamic_cast<SphereCollider*>(c1);
	AABB* a = dynamic_cast<AABB*>(c2);
	Vector3 center = sc->GetPosition();
	float radius = sc->GetRadius();
	Vector3 max = a->GetMax() + a->GetPosition();
	Vector3 min = a->GetMin() + a->GetPosition();

	float dx = min.x - center.x < 0.0f ? 0.0f : min.x - center.x;
	dx = dx < center.x - max.x ? center.x - max.x : dx;
	float dy = min.y - center.y < 0.0f ? 0.0f : min.y - center.y;
	dy = dy < center.y - max.y ? center.y - max.y : dy;
	float dz = min.z - center.z < 0.0f ? 0.0f : min.z - center.z;
	dz = dz < center.z - max.z ? center.z - max.z : dz;
	float distSq = dx * dx + dy * dy + dz * dz;

	if (distSq <= radius * radius)
	{
		c1->m_isCollision = true;
		c2->m_isCollision = true;
		c1->m_isCollisionThisFrame = true;
		c2->m_isCollisionThisFrame = true;
		for (int i = 0; i < c1->m_hitObject.size(); i++)
		{
			if (c1->m_hitObject[i] == nullptr)
			{
				c1->m_hitObject[i] = c2->GetGameObject();
			}
		}
		for (int i = 0; i < c2->m_hitObject.size(); i++)
		{
			if (c2->m_hitObject[i] == nullptr)
			{
				c2->m_hitObject[i] = c1->GetGameObject();
			}
		}
	}
}

void Collider::LoadProperties(const rapidjson::Value& inProp)
{
	return;
}

void Collider::SavePropaties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	return;
}