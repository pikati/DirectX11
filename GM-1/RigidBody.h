#pragma once
#include "Component.h"

enum class ForceMode : int
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
	bool m_isKinematic = false;
public:

	void SystemInitialize() override;
	void FixedUpdate() override;
	void Finalize() override;

	void SetGravity(float gravity);
	float GetGrabity();
	void AddForce(Vector3 force, ForceMode forceMode = ForceMode::FORCE);
	void SetForce(Vector3 force);
	void SetVelocity(Vector3 velocity);
	Vector3 GetForce();
	Vector3 GetVelocity();
	void SetKinematic(bool on);
	bool IsKinematic();
	void DrawInformation() override;

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

