#pragma once
#include "main.h"
#include "Component.h"

class Camera : public Component
{
private:
	Vector3 m_target;
	D3DXMATRIX m_viewMatrix;
public:
	Camera();
	~Camera();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	Vector3 GetTarget();
	D3DXMATRIX GetViewMatrix();
};

