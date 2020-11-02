#pragma once
#include "Component.h"
class AABB;

class WoodBox : public Component
{
private:
	Vector3 m_velocity = Vector3::zero;
	bool m_isGrounded = false;
	AABB* m_collider = nullptr;
	Vector3 m_startPosition = Vector3::zero;
	float m_gravity = 9.8f;
public:
	void Initialize() override;
	void Update() override;
	void OnCollisionEnter(GameObject* obj) override;
	void OnCollisionExit(GameObject* obj) override;

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

