#include "SphereCollider.h"
#include "Transform.h"
#include "LevelLoader.h"
#include "rapidjson/document.h"

SphereCollider::SphereCollider()
{
	m_colliderType = Sphere;
	if (m_transform == nullptr)
	{
		m_transform = new Transform();
		m_sortingOrder = 0;
	}
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
	Collider::Update();
	//hitObject = nullptr;
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

void SphereCollider::SetProperties(Component* c)
{
	SphereCollider* s = dynamic_cast<SphereCollider*>(c);
	m_center = s->m_center;
	m_radius = s->m_radius;
}

void SphereCollider::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetVector3(inProp, "center", m_center);
	JsonHelper::GetFloat(inProp, "radius", m_radius);
	JsonHelper::GetVector3(inProp,  "position", m_transform->position);
	JsonHelper::GetVector3(inProp,  "rotation", m_transform->rotation);
	JsonHelper::GetVector3(inProp,  "scale", m_transform->scale);
}

void SphereCollider::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddVector3(alloc, inProp, "center", m_center);
	JsonHelper::AddFloat(alloc, inProp, "radius", m_radius);
	JsonHelper::AddVector3(alloc, inProp, "position", m_transform->position);
	JsonHelper::AddVector3(alloc, inProp, "rotation", m_transform->rotation);
	JsonHelper::AddVector3(alloc, inProp, "scale", m_transform->scale);
}