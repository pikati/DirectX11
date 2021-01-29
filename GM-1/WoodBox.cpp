#include "WoodBox.h"
#include "AABB.h"
#include "GameObject.h"
#include "Transform.h"
#include "FPS.h"
#include "LevelLoader.h"

void WoodBox::Initialize()
{
	m_collider = gameObject->GetComponent<AABB>();
}

void WoodBox::Update()
{
	if (gameObject->transform->position.y < -20)
	{
		gameObject->transform->position = m_startPosition;
	}
}

void WoodBox::OnCollisionEnter(GameObject* obj)
{
	if (obj->tag == "Ground")
	{
		m_isGrounded = true;
	}
}

void WoodBox::OnCollisionExit(GameObject* obj)
{
	if (obj->tag == "Ground")
	{
		m_isGrounded = false;
	}
}

void WoodBox::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetVector3(inProp, "startPosition", m_startPosition);
	JsonHelper::GetInt(inProp, "id", m_id);
}

void WoodBox::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddVector3(alloc, inProp, "startPosition", m_startPosition);
	JsonHelper::AddInt(alloc, inProp, "id", m_id);
}