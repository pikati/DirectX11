#pragma once
#include "main.h"
#include "Engine.h"

class EditorCamera
{
private:
	D3D11_VIEWPORT m_viewPort;
	Vector3 m_target;
	Vector3 m_position;
	Vector3 m_rotation;
	Vector2 m_mousePosition;
	Vector2 m_moveValue;
	Vector2 m_oldMoveValue;
	Vector3 m_up;
	Vector3 m_offset = { 0, 10.0f, -10.0f };
	float m_distance = 10;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;
	bool m_isMoveVertical = false;

	void Dolly();
	void Scroll();

	void CalcUp();

	void DebugLogRotation();
	void DebugLogPosition();
public:
	~EditorCamera() {};
	void Initialize();
	void Update();
	void Draw();
	const float& GetDistance();
	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjectionMatrix();
};

