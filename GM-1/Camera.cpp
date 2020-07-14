#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "Camera.h"
#include "Transform.h"

Camera::Camera()
{

}

Camera::~Camera()
{
	
}

void Camera::Initialize()
{
	gameObject->transform->position = Vector3(0.0f, 5.0f, -5.0f);
	m_target = Vector3(0.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
	if (CInput::GetKeyPress('J'))
	{
		gameObject->transform->position += Vector3(-0.1f, 0.0f, 0.0f);
		m_target += Vector3(-0.1f, 0.0f, 0.0f);
	}
	if (CInput::GetKeyPress('L'))
	{
		gameObject->transform->position += Vector3(0.1f, 0.0f, 0.0f);
		m_target += Vector3(0.1f, 0.0f, 0.0f);
	}
	if (CInput::GetKeyPress('I'))
	{
		gameObject->transform->position += Vector3(0.0f, 0.0f, 0.1f);
		m_target += Vector3(0.0f, 0.0f, 0.1f);
	}
	if (CInput::GetKeyPress('K'))
	{
		gameObject->transform->position += Vector3(0.0f, 0.0f, -0.1f);
		m_target += Vector3(0.0f, 0.0f, -0.1f);
	}
}

void Camera::Draw()
{
	//ビューマトリクス設定
	D3DXMatrixLookAtLH(&m_viewMatrix, &D3DXVECTOR3(gameObject->transform->position.x, gameObject->transform->position.y, gameObject->transform->position.z), &D3DXVECTOR3(m_target.x, m_target.y, m_target.z), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CRenderer::SetViewMatrix(&m_viewMatrix);

	//プロジェクションマトリクス設定
	D3DXMATRIX projctionMatrix;
	D3DXMatrixPerspectiveFovLH(&projctionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&projctionMatrix);
}

void Camera::Finalize()
{

}

Vector3 Camera::GetTarget()
{
	return m_target;
}

D3DXMATRIX Camera::GetViewMatrix()
{
	return m_viewMatrix;
}