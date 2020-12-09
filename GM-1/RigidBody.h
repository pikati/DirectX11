#pragma once
#include "Component.h"
enum class FORCE_MODE : int
{
	FORCE,
	IMPULSE,
	MAX
};
class RigidBody : public Component
{
private:
	Vector3 m_force = { 0,0,0 };
	Vector3 m_oldForce = { 0,0,0 };
	float m_mass = 1.0f;
	float m_gravity = -9.8f;
	Vector3 m_velocity = { 0,0,0 };
	Vector3 m_impulseForce = { 0,0,0 };
public:

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void SetGravity(float gravity);
	float GetGrabity();
	void AddForce(Vector3 force, FORCE_MODE forceMode = FORCE_MODE::FORCE);
	void SetForce(Vector3 force);
	Vector3 GetForce();
	Vector3 GetVelocity();

	void DrawInformation() override;
};
