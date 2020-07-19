#pragma once
#include "main.h"
#include "Component.h"

class GameObject;

class Camera : public Component
{
private:
	Vector3 m_target;
	D3DXMATRIX m_viewMatrix;
	GameObject* m_player;
	bool m_inisialized = false;
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

