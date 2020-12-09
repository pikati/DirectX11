#pragma once
#include "main.h"
#include "Component.h"

class Camera;
class Gameobject;

class CameraController : public Component
{
private:
	Camera* m_camera;
	Vector3 m_target;
	bool m_inisialized = false;
	float m_rotation = 0;
	const float m_rotationValue = D3DX_PI * 0.01f;
	const float m_distance = 10.0f; 
	GameObject* m_player;
public:
	void Initialize() override;
	void Update() override;
	Vector3 GetTarget();

	float GetRotation();
};

