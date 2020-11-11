#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "AABB.h"
#include <math.h>
#include "ImguiManager.h"
#include "MeshCollider.h"
#include "BoxCollider.h"

std::vector<Collider*> Collider::m_colliders;

void Collider::Initialize()
{

}

void Collider::Update()
{
	if (m_isCollision == false)
	{
		if (m_collisionObject != nullptr)
		{
			gameObject->OnCollisionExit(m_collisionObject);
			m_collisionObject = nullptr;
		}
	}
}

void Collider::Draw()
{

}

void Collider::Finalize()
{

}

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
	if (colliderID < 0)
	{
		return;
	}
	m_colliders.erase(m_colliders.begin() + colliderID);
	for (unsigned int i = colliderID; i < m_colliders.size(); i++)
	{
		m_colliders[i]->m_colliderID--;
	}
}

void Collider::UpdateCollision()
{
	for (unsigned int i = 0; i < m_colliders.size(); i++)
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
	for (unsigned int i = 0; i < m_colliders.size(); i++)
	{
		//���̃t���[���ɓ�����Ȃ������甲���������������
		/*if (!m_colliders[i]->m_isCollisionThisFrame)
		{
			m_colliders[i]->m_exitObject = m_colliders[i]->m_hitObject;
			for (unsigned int j = 0; j < m_colliders[i]->m_hitObject.size(); j++)
			{
				m_colliders[i]->m_hitObject[j] = nullptr;
			}
		}*/
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
	else if (c1->m_colliderType == Box && c2->m_colliderType == Sphere)
	{
		Box2Sphere(c1, c2);
	}
	else if (c1->m_colliderType == Sphere && c2->m_colliderType == Box)
	{
		Box2Sphere(c2, c1);
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
		c1->gameObject->OnCollisionEnter(c2->gameObject);
		c2->gameObject->OnCollisionEnter(c1->gameObject);
		c1->m_collisionObject = c2->gameObject;
		c2->m_collisionObject = c1->gameObject;
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
	c1->gameObject->OnCollisionEnter(c2->gameObject);
	c2->gameObject->OnCollisionEnter(c1->gameObject);
	c1->m_collisionObject = c2->gameObject;
	c2->m_collisionObject = c1->gameObject;

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
			pos2.x -= dx + 0.01f;
		}
		else if (a2Kinematic)
		{
			pos1.x += dx + 0.01f;
		}
		else if(!a1Kinematic && !a2Kinematic)
		{
			pos1.x += dx / 2.0f + 0.01f;
			pos2.x -= dx / 2.0f + 0.01f;
		}
	}
	else if (fabsf(dy) <= fabsf(dx) && fabsf(dy) <= fabsf(dz))
	{
		if (a1Kinematic)
		{
			pos2.y -= dy + 0.01f;
		}
		else if (a2Kinematic)
		{
			pos1.y += dy + 0.01f;
		}
		else if (!a1Kinematic && !a2Kinematic)
		{
			pos1.y += dy / 2.0f + 0.01f;
			pos2.y -= dy / 2.0f + 0.01f;
		}
	}
	else
	{
		if (a1Kinematic)
		{
			pos2.z -= dz + 0.01f;
		}
		else if (a2Kinematic)
		{
			pos1.z += dz + 0.01f;
		}
		else if (!a1Kinematic && !a2Kinematic)
		{
			pos1.z += dz / 2.0f + 0.01f;
			pos2.z -= dz / 2.0f + 0.01f;
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
		c1->gameObject->OnCollisionEnter(c2->gameObject);
		c2->gameObject->OnCollisionEnter(c1->gameObject);
		c1->m_collisionObject = c2->gameObject;
		c2->m_collisionObject = c1->gameObject;
	}
}

void Collider::Box2Mesh(Collider* c1, Collider* c2)
{
	AABB* a = dynamic_cast<AABB*>(c1);
	MeshCollider* m = dynamic_cast<MeshCollider*>(c2);
	Vector3 pos = a->GetPosition();
	float height = a->GetHeight();
	Vector3* vertices = m->GetVertices();
	int sizeX = m->GetSizeX();
	int sizeZ = m->GetSizeZ();
	int vSize = (sizeX + 1) * (sizeZ + 1);
	for (int i = 0; i < vSize; i++)
	{
		vertices[i] += c2->gameObject->transform->position;
	}
	float minx = vertices[0].x;
	float maxx;
	if (minx < vertices[vSize - 1].x)
	{
		maxx = vertices[vSize - 1].x;
	}
	else
	{
		minx = vertices[vSize - 1].x;
		maxx = vertices[0].x;
	}

	float minz = vertices[0].z;
	float maxz;
	if (minz < vertices[vSize - 1].z)
	{
		maxz = vertices[vSize - 1].z;
	}
	else
	{
		minz = vertices[vSize - 1].z;
		maxz = vertices[0].z;
	}
	if (pos.x > maxx || pos.x < minx || pos.z > maxz || pos.z <= minz)
	{
		return;
	}

	for (int z = 0; z < sizeZ; z++)
	{
		for (int x = 0; x < sizeX + 1; x += 2)
		{
			Vector3 p1 = pos + Vector3(0, height, 0);
			Vector3 p2 = pos;
			Vector3 v0 = vertices[x];
			Vector3 v1 = vertices[x + 1];
			Vector3 v2 = vertices[x + (z + 1) * (sizeX + 1)];
			/*Vector3 v23 = vertices[x + 1] - vertices[x + (z + 1) * (sizeX + 1)];
			Vector3 v12 = vertices[x] - vertices[x + 1];
			v23.Normalize();
			v12.Normalize();
			Vector3 vn = Vector3::Cross(v12, v23);
			if (vn.y < 0) vn.y *= -1;
			vn.Normalize();
			Vector3 v1 = pos - vertices[x];
			Vector3 v2 = p2 - vertices[x];
			float dd1 = Vector3::Dot(v1, vn);
			float dd2 = Vector3::Dot(v2, vn);*/
			//�����Ƃ���|���S���̃T�C�Y�𖳌��ɂ������ʂ��Ԃ������ɂ�
			bool isCollision = CollisionLineSegumentAndPlane(p1, p2, v0, v1, v2);
			if (/*dd1 * dd2 <= 0*/isCollision)
			{
				//float length = Vector3::Cross(v12, v23).Length();
				//float d1 = abs(Vector3::Dot(vn, v1)) / length;
				//float d2 = abs(Vector3::Dot(vn, v2)) / length;
				//float a = d1 / (d1 + d2);
				//Vector3 v3 = (1 - a) * v1 + a * v2;
				//Vector3 collisionPoint = vertices[x] + v3;

				////�|���S�����Ɍ������_�����邩�`�F�b�N
				//Vector3 v31 = vertices[x + (z + 1) * (sizeX + 1)] - vertices[x];

				//v31.Normalize();
				//Vector3 n11 = Vector3::Cross(v12, collisionPoint);
				//Vector3 n22 = Vector3::Cross(v23, collisionPoint);
				//Vector3 n33 = Vector3::Cross(v31, collisionPoint);
				//Vector3 n1 = Vector3::Cross(vertices[x + 1] - vertices[x], collisionPoint - vertices[x + 1]);
				//Vector3 n2 = Vector3::Cross(vertices[x + (z + 1) * (sizeX + 1)] - vertices[x + 1], collisionPoint - vertices[x + (z + 1) * (sizeX + 1)]);
				//Vector3 n3 = Vector3::Cross(vertices[x] - vertices[x + (z + 1) * (sizeX + 1)], collisionPoint);
				//n1.Normalize();
				//n2.Normalize();
				//n3.Normalize();
				//float r1 = 1.0f - Vector3::Dot(vn, n1);
				//float r2 = 1.0f - Vector3::Dot(vn, n2);
				//float r3 = 1.0f - Vector3::Dot(vn, n3);
				//if (r1 > 0.001f) continue;
				//if (r2 > 0.001f) continue;
				//if (r3 > 0.001f) continue;
				Vector3 p = CalcIntersectionLineSegmentAndPlane(p1, p2, v0, v1, v2);

				if (DetectPointIsEnclosedByPolygon(p, v0, v1, v2))
				{
					c1->gameObject->transform->position.y = p.y;
					c1->m_isCollision = true;
					c2->m_isCollision = true;
					c1->m_isCollisionThisFrame = true;
					c2->m_isCollisionThisFrame = true;
					c1->gameObject->OnCollisionEnter(c2->gameObject);
					c2->gameObject->OnCollisionEnter(c1->gameObject);
					c1->m_collisionObject = c2->gameObject;
					c2->m_collisionObject = c1->gameObject;
				}
			}

			v0 = vertices[x];
			v1 = vertices[x + (z + 1) * (sizeX + 1)];
			v2 = vertices[x - 1 + (z + 1) * (sizeX + 1)];
			isCollision = CollisionLineSegumentAndPlane(p1, p2, v0, v1, v2);
			if (isCollision)
			{
				Vector3 p = CalcIntersectionLineSegmentAndPlane(p1, p2, v0, v1, v2);

				if (DetectPointIsEnclosedByPolygon(p, v0, v1, v2))
				{
					c1->gameObject->transform->position.y = p.y;
					c1->m_isCollision = true;
					c2->m_isCollision = true;
					c1->m_isCollisionThisFrame = true;
					c2->m_isCollisionThisFrame = true;
					c1->gameObject->OnCollisionEnter(c2->gameObject);
					c2->gameObject->OnCollisionEnter(c1->gameObject);
					c1->m_collisionObject = c2->gameObject;
					c2->m_collisionObject = c1->gameObject;
				}
			}
		}
	}
}

void Collider::Box2Sphere(Collider* c1, Collider* c2)
{
	BoxCollider* box = dynamic_cast<BoxCollider*>(c1);
	SphereCollider* s = dynamic_cast<SphereCollider*>(c2);

	Vector3 vec = Vector3::zero;

	Vector3 sPos = s->GetPosition();
	float radius = s->GetRadius();
	sPos.y += radius;
	Vector3 bPos = box->GetPosition();
	//bPos.y += box->GetLength(1);
	Vector3 direction = sPos - bPos;
	Vector3 colLength;

	for (int i = 0; i < 3; i++)
	{
		float len = box->GetLength(i);//����
		Vector3 obb = box->GetAxis(i);//�P�ʃx�N�g��
		obb /= len;
		float dist = fabs(Vector3::Dot(obb, direction));
		/*dist = fabs(dist);
		if (dist > 1)
		{
			vec += (1 - dist) * len * obb;
		}*/
		if (i == 0)
		{
			vec.x = dist;
			colLength.x = len - radius;
		}
		else if (i == 1)
		{
			vec.y = dist; 
			colLength.y = len - radius;
		}
		else if (i == 2)
		{
			vec.z = dist;
			colLength.z = len - radius;
		}
	}

	if (vec.x <= colLength.x && vec.y <= colLength.y && vec.z <= colLength.z)
	{
		c1->m_isCollision = true;
		c2->m_isCollision = true;
		c1->m_isCollisionThisFrame = true;
		c2->m_isCollisionThisFrame = true;
		c1->gameObject->OnCollisionEnter(c2->gameObject);
		c2->gameObject->OnCollisionEnter(c1->gameObject);
		c1->m_collisionObject = c2->gameObject;
		c2->m_collisionObject = c1->gameObject;
	}
}

bool Collider::CollisionLineSegumentAndPlane(Vector3 a, Vector3 b, Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 n = Vector3::Normalize(Vector3::Cross(v1 - v0, v2 - v1));
	Vector3 g = (v0 + v1 + v2) * (1.0f / 3.0f);
	float d1 = Vector3::Dot((a - g), n);
	float d2 = Vector3::Dot((b - g), n);
	if (Vector3::Dot((a - g), n) * Vector3::Dot((b - g), n) <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vector3 Collider::CalcIntersectionLineSegmentAndPlane(Vector3 a, Vector3 b, Vector3 v0, Vector3 v1, Vector3 v2)
{
	float distAP = CalcDistancePointAndPlane(a, v0, v1, v2);
	float distBP = CalcDistancePointAndPlane(b, v0, v1, v2);

	float t = distAP / (distAP + distBP);
	Vector3 n = (b - a) * t + a;
	return (b - a) * t + a;
}

float Collider::CalcDistancePointAndPlane(Vector3 p, Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 n = Vector3::Normalize(Vector3::Cross(v1 - v0, v2 - v1));
	Vector3 g = (v0 + v1 + v2) * (1.0f / 3.0f);
	float a = abs(Vector3::Dot(n, p - g));
	return abs(Vector3::Dot(n, p - g));
}

bool Collider::DetectPointIsEnclosedByPolygon(Vector3 p, Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 n = Vector3::Normalize(Vector3::Cross(v1 - v0, v2 - v1));

	Vector3 n0 = Vector3::Normalize(Vector3::Cross(v1 - v0, p - v1));
	Vector3 n1 = Vector3::Normalize(Vector3::Cross(v2 - v1, p - v2));
	Vector3 n2 = Vector3::Normalize(Vector3::Cross(v0 - v2, p - v0));

	if ((1.0f - Vector3::Dot(n, n0)) > 0.001f) return false;
	if ((1.0f - Vector3::Dot(n, n1)) > 0.001f) return false;
	if ((1.0f - Vector3::Dot(n, n2)) > 0.001f) return false;

	return true;
}


void Collider::LoadProperties(const rapidjson::Value& inProp)
{
	return;
}

void Collider::SavePropaties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	return;
}