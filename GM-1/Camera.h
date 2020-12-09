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
	int m_viewPortWidth = 0;
	int m_viewPortHeight = 0;
	int m_viewPortTopLeftX = 0;
	int m_viewPortTopLeftY = 0;
	int m_renderNum = 0;
	D3D11_VIEWPORT m_viewPort;
	
	void CheckView();
public:
	Camera();
	~Camera();
	void Initialize();
	void Update();
	void Draw();
	D3DXMATRIX GetViewMatrix();
	void SetLookAt(Vector3 lookPoint);
	void DrawInformation() override;

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

