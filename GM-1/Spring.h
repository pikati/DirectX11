#pragma once
#include "Component.h"
class RigidBody;

class Spring : public Component
{
private:
	RigidBody* m_rigidBody = nullptr;
	int m_boundCount = 0;
	float m_reflectionCoe = 4.0f;
	float m_attenuationCoe = 1.0f;//å∏êäåWêî
	float m_boundPos = 0.0f;
	Vector3 m_rotationValue = { 0,0,0 };

	void Bound();
	void Reset();
public:
	void Initialize() override;
	void Update() override;
};

