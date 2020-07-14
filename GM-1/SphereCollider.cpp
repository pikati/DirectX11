#include "SphereCollider.h"
#include "Transform.h"

SphereCollider::SphereCollider()
{
	m_transform = new Transform();
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Initialize()
{
	m_colliderID = SetCollider(this);
}

void SphereCollider::Update()
{
	m_isCollisionThisFrame = false;
	hitObject = nullptr;
}

void SphereCollider::Draw()
{
	
}

void SphereCollider::Finalize()
{
	DeleteCollider(m_colliderID);
	delete m_transform;
}

void SphereCollider::SetCenter(Vector3 center)
{
	m_center = center;
}

void SphereCollider::SetCenter(float x, float y, float z)
{
	m_center.Set(x, y, z);
}

void SphereCollider::SetRadius(float radius)
{
	m_radius = radius;
}

Vector3 SphereCollider::GetPosition()
{
	return m_center + gameObject->transform->position;
}

float SphereCollider::GetRadius()
{
	return m_radius;
}