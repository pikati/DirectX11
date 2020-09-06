#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
#include "Player.h"
#include "Scene.h"

static D3DMATRIX m;

Camera::Camera()
{

}

Camera::~Camera()
{
	
}

void Camera::Initialize()
{
	gameObject->transform->position = Vector3(0.0f, 5.0f, -10.0f);
	m_target = Vector3(0.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
	if (!m_inisialized)
	{
		Scene* s = CManager::GetScene();
		m_player = s->Find<Player>();
		m_inisialized = true;
	}
	m_target = m_player->transform->position;
	float pZ = m_player->transform->position.z;


	if (CInput::GetKeyPress('J'))
	{
		m_rotation -= m_rotationValue;
		if (m_rotation < 0)
		{
			m_rotation += D3DX_PI * 2.0f;
		}
	}
	if (CInput::GetKeyPress('L'))
	{
		m_rotation += m_rotationValue;
		if (m_rotation > D3DX_PI * 2.0f)
		{
			m_rotation -= D3DX_PI * 2.0f;
		}
	}
	gameObject->transform->position.x = m_target.x - sinf(m_rotation) * m_distance;
	gameObject->transform->position.y = m_player->transform->position.y + 5.0f;
	gameObject->transform->position.z = m_target.z - cosf(m_rotation) * m_distance;
}

void Camera::Draw()
{
	//ビューマトリクス設定
	D3DXMatrixLookAtLH(&m_viewMatrix, &D3DXVECTOR3(gameObject->transform->position.x, gameObject->transform->position.y, gameObject->transform->position.z), &D3DXVECTOR3(m_target.x, m_target.y, m_target.z), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CRenderer::SetViewMatrix(&m_viewMatrix);
	m = m_viewMatrix;
	//プロジェクションマトリクス設定
	D3DXMATRIX projctionMatrix;
	D3DXMatrixPerspectiveFovLH(&projctionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&projctionMatrix);
}

Vector3 Camera::GetTarget()
{
	return m_target;
}

D3DXMATRIX Camera::GetViewMatrix()
{
	return m;
}

float Camera::GetRotation()
{
	return m_rotation;
}