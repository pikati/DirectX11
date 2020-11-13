#pragma once
#include "main.h"
#include "Component.h"

class GameObject;

class Camera : public Component
{
private:
	Vector3 m_target;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;
	GameObject* m_player;
	bool m_inisialized = false;
	float m_rotation = 0;
	const float m_rotationValue = D3DX_PI * 0.01f;
	const float m_distance = 10.0f;
	
	bool CheckView();
public:
	Camera();
	~Camera();
	void Initialize();
	void Update();
	void Draw();
	Vector3 GetTarget();
	D3DXMATRIX GetViewMatrix();
	float GetRotation();
};

