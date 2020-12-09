#include "Spring.h"
#include "GameObject.h"
#include "Transform.h"
#include "RigidBody.h"
#include <random>

void Spring::Initialize()
{
	m_boundCount = 0;
	m_rigidBody = gameObject->GetComponent<RigidBody>();
	std::random_device rnd;
	float x = rnd() % 80 * 0.1f - 4;
	float y = rnd() % 10 * 0.1f + 1;
	float z = rnd() % 80 * 0.1f - 4;
	gameObject->transform->position.Set(x, y, z);
	m_rotationValue.Set(x, y, z);
}

void Spring::Update()
{
	gameObject->transform->rotation += m_rotationValue;
	if (gameObject->transform->position.y <= m_boundPos - 1)
	{
		if (m_boundCount < 3)
		{
			Bound();
		}
	}
	if (gameObject->transform->position.y <= m_boundPos - 2)
	{
		if (m_boundCount >= 3)
		{
			Reset();
		}
	}
}

void Spring::Bound()
{
	std::random_device rnd;
	float x = rnd() % 40 * 0.1f - 2;
	float z = rnd() % 40 * 0.1f - 2;
	float a = m_reflectionCoe * (-gameObject->transform->position.y);
	float v = m_rigidBody->GetVelocity().y;
	float y = a - m_rigidBody->GetVelocity().y * m_attenuationCoe;
	m_rigidBody->AddForce(Vector3(x, y, z), FORCE_MODE::IMPULSE);
	m_boundCount++;

}

void Spring::Reset()
{
	std::random_device rnd;
	float x = rnd() % 80 * 0.1f - 4;
	float y = rnd() % 10 * 0.1f + 1;
	float z = rnd() % 80 * 0.1f - 4;
	gameObject->transform->position.Set(x, y, z);
	m_rotationValue.Set(x, y, z);
	m_boundCount = 0;
	m_rigidBody->Initialize();
}