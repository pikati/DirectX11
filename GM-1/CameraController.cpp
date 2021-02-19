#include "CameraController.h"
#include "manager.h"
#include "Scene.h"
#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "input.h"
#include "Camera.h"

void CameraController::Initialize()
{
	m_target = Vector3(0.0f, 0.0f, 0.0f);
	m_inisialized = false;
	m_camera = gameObject->GetComponent<Camera>();

}

void CameraController::Update()
{
	if (!m_inisialized)
	{
		Scene* s = CManager::GetScene();
		m_player = s->Find<Player>();
		m_inisialized = true;
	}
	if (m_player == nullptr)
	{
		m_target = { 0,0,0 };
	}
	else
	{
		m_target = m_player->transform->position;
	}
	float pZ = m_target.z;


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

	/*if (gameObject->tag == "MainCamera")
	{
		if (CInput::GetKeyPress('Z'))
		{
			m_target.x += 0.4f;
		}
		if (CInput::GetKeyPress('X'))
		{
			m_target.x += -0.4f;
		}
		if (CInput::GetKeyPress('N'))
		{
			m_target.y += 0.4f;
		}
		if (CInput::GetKeyPress('M'))
		{
			m_target.y += -0.4f;
		}
	}*/
	if (m_camera)
	{
		m_camera->SetLookAt(m_target);
	}
}

Vector3 CameraController::GetTarget()
{
	return m_target;
}

float CameraController::GetRotation()
{
	return m_rotation;
}