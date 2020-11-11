#pragma once
#include "Component.h"
#include "Engine.h"

class Transform : public Component
{
private:
	Vector3 m_oldPosition;
	Vector3 m_oldRotation;
	Vector3 m_oldScale;
	bool m_isChangePosition;
	bool m_isChangeRotation;
	bool m_isChangeScale;
public:
	Transform();
	~Transform();
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	void Update() override;

	bool IsChangePosition();
	bool IsChangeRotation();
	bool IsChangeScale();

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

