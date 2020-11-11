#include "Transform.h"
#include "LevelLoader.h"
#include "rapidjson/document.h"

Transform::Transform()
{
	position = Vector3::zero;
	rotation = Vector3::zero;
	scale = Vector3::one;
	m_oldPosition = position;
	m_oldRotation = rotation;
	m_oldScale = scale;
	m_sortingOrder = 0;
}

Transform::~Transform()
{

}

void Transform::Update()
{
	if (position != m_oldPosition)
	{
		m_isChangePosition = true;
	}
	else
	{
		m_isChangePosition = false;
	}
	if (rotation != m_oldRotation)
	{
		m_isChangeRotation = true;
	}
	else
	{
		m_isChangeRotation = false;
	}
	if (scale != m_oldScale)
	{
		m_isChangeScale = true;
	}
	else
	{
		m_isChangeScale = false;
	}
	m_oldPosition = position;
	m_oldRotation = rotation;
	m_oldScale = scale;
}

bool Transform::IsChangePosition()
{
	return m_isChangePosition;
}

bool Transform::IsChangeRotation()
{
	return m_isChangeRotation;
}

bool Transform::IsChangeScale()
{
	return m_isChangeScale;
}

void Transform::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetVector3(inProp, "position", position);
	JsonHelper::GetVector3(inProp, "rotation", rotation);
	JsonHelper::GetVector3(inProp, "scale", scale);
}

void Transform::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddVector3(alloc, inProp, "position", position);
	JsonHelper::AddVector3(alloc, inProp, "rotation", rotation);
	JsonHelper::AddVector3(alloc, inProp, "scale", scale);
}