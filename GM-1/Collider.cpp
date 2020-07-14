#include "Collider.h"
#include "GameObject.h"

std::vector<Collider*> Collider::m_colliders;

bool Collider::IsCollision()
{
	return m_isCollision;
}

int Collider::SetCollider(Collider* collider)
{
	m_colliders.push_back(collider);
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
	int a = m_colliders.size() - 1;
	int b = a + 1;
	for (int i = 0; i < a; i++)
	{
		for (int j = i + 1; j < b; j++)
		{
			RunCollisionDetection(m_colliders[i], m_colliders[j]);
		}
	}
}

void Collider::RunCollisionDetection(Collider* c1, Collider* c2)
{
	Vector3 c1Position = c1->GetPosition();
	float c1Radius = c1->GetRadius();
	Vector3 c2Position = c2->GetPosition();
	float c2Radius = c2->GetRadius();

	if (powf(c2Position.x - c1Position.x, 2.0f) + powf(c2Position.y - c1Position.y, 2.0f) + powf(c2Position.z - c1Position.z, 2.0f) <= powf(c1Radius + c2Radius, 2.0f))
	{
		if (c1->m_isCollisionThisFrame || c2->m_isCollisionThisFrame) return;
		c1->m_isCollision = true;
		c2->m_isCollision = true;
		c1->m_isCollisionThisFrame = true;
		c2->m_isCollisionThisFrame = true;
		c1->hitObject = c2->GetGameObject();
		c2->hitObject = c1->GetGameObject();
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

void Collider::Destroy()
{
	m_isDestroy = true;
}

float Collider::GetRadius()
{
	return 0;
}

GameObject* Collider::GetHitGameObject()
{
	return hitObject;
}