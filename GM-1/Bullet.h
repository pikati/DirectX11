#pragma once
#include "Component.h"


class Bullet : public Component
{
private:
	float m_speed = 2.5f;
	Vector3 m_direction = Vector3::zero;
	GameObject* m_player = nullptr;
	int m_lifeTime = 180;
public:
	void Initialize() override;
	void Update() override;

	void OnCollisionEnter(GameObject* obj) override;
};

