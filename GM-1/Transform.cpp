#include "Transform.h"
#include "LevelLoader.h"
#include "rapidjson/document.h"

Transform::Transform()
{
	position = Vector3::zero;
	rotation = Vector3::zero;
	scale = Vector3::one;
	m_sortingOrder = 0;
}

Transform::~Transform()
{

}

void Transform::Initialize()
{
}

void Transform::Update()
{

}

void Transform::Draw()
{

}

void Transform::Finalize()
{

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