#pragma once
#include "Component.h"
#include "Engine.h"

class Transform : public Component
{
public:
	Transform();
	~Transform();
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

