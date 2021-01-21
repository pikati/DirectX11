#include "EditorCamera.h"
#include "renderer.h"
#include "input.h"
#include "FPS.h"
#include <DirectXMath.h>
#include <string>

#define MOVE_SPEED 0.05f

void EditorCamera::Initialize()
{
	m_target = { 0, 0, 0 };
	m_position = { 0, 10, -10 };
	m_rotation = { 0, 180, 0 };
	m_up = { 0, 1, 0 };
	m_viewPort.Height = 720;
	m_viewPort.Width = 1280;
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_distance = 0;
}

void EditorCamera::Update()
{
	CRenderer::SetCameraPosition(m_position);
	m_mousePosition = CInput::GetMousePosition();
	m_moveValue = m_mousePosition - m_oldMoveValue;
	Dolly();
	Scroll();
	m_oldMoveValue = m_mousePosition;
	if (CInput::GetMouseTrigger(0))
	{
		DebugLogPosition();
	}
}

void EditorCamera::Draw()
{
	CRenderer::SetViewPort(&m_viewPort);
	D3DXMATRIX v;
	D3DXMATRIX p;	
	D3DXMatrixLookAtLH(&v, &D3DXVECTOR3(m_position.x, m_position.y, m_position.z), &D3DXVECTOR3(m_target.x, m_target.y, m_target.z), &D3DXVECTOR3(m_up.x, m_up.y, m_up.z));
	CRenderer::SetViewMatrix(&v);
	m_viewMatrix = v;
	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&p, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&p);
	m_projectionMatrix = p;
}

void EditorCamera::Dolly()
{
	using namespace DirectX;
	if (CInput::GetMouseTrigger(1))
	{
		float inputx = fabsf(m_moveValue.x);
		float inputy = fabsf(m_moveValue.y);
		if (inputx < inputy)
		{
			m_isMoveVertical = true;
		}
		else
		{
			m_isMoveVertical = false;
		}
	}
	if (CInput::GetMouseDown(1))
	{
		if (m_isMoveVertical)
		{
			m_rotation.x += m_moveValue.y * MOVE_SPEED;
		}
		else
		{
			m_rotation.y += m_moveValue.x * MOVE_SPEED;
		}
		/*float x = sinf(XMConvertToRadians(m_rotation.y)) * distance + m_target.x;
		float y = cosf(XMConvertToRadians(m_rotation.x)) * distance + m_target.y;
		float z = cosf(XMConvertToRadians(m_rotation.y)) * distance * cosf(XMConvertToRadians(m_rotation.x)) + m_target.z;*/
		float rotx = XMConvertToRadians(-m_rotation.x);
		float roty = XMConvertToRadians(-m_rotation.y);
		/*float x = (cosf(roty) + sinf(roty)) * distance + m_target.x;
		float y = (-sinf(rotx) * -sinf(roty) + cosf(rotx) + -sinf(rotx) * cosf(roty)) * distance + m_target.y;
		float z = (cosf(rotx) * -sinf(roty) + sinf(rotx) + cosf(rotx) * cosf(roty)) * distance + m_target.z;*/
		float x = sinf(roty) * m_distance + m_target.x;
		float y = m_distance + m_target.y;
		float z = cosf(roty) * m_distance + m_target.z;
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
		//CalcUp();
		DebugLogPosition();
	}
	if (m_rotation.x< 0)
	{
		m_rotation.x += 360;
	}
	else if (m_rotation.x >= 360)
	{
		m_rotation.x -= 360;
	}
	if (m_rotation.y < 0)
	{
		m_rotation.y += 360;
	}
	else if (m_rotation.y >= 360)
	{
		m_rotation.y -= 360;
	}

}

void EditorCamera::Scroll()
{
	using namespace DirectX;
	if (CInput::GetMouseTrigger(2))
	{
		float inputx = fabsf(m_moveValue.x);
		float inputy = fabsf(m_moveValue.y);
		if (inputx < inputy)
		{
			m_isMoveVertical = true;
		}
		else
		{
			m_isMoveVertical = false;
		}
	}
	if (CInput::GetMouseDown(2))
	{
		float x = m_moveValue.x * cosf(XMConvertToRadians(m_rotation.y)) * MOVE_SPEED;
		float y = m_moveValue.y * cosf(XMConvertToRadians(m_rotation.x)) * MOVE_SPEED;
		if (m_isMoveVertical)
		{
			x = 0;
		}
		else
		{
			y = 0;
		}
		float z = m_moveValue.x * (sinf(XMConvertToRadians(m_rotation.y)) + m_moveValue.y * sinf(XMConvertToRadians(m_rotation.x))) * MOVE_SPEED;
		m_position.x += x;
		m_position.y += y;
		m_position.z += z;
		m_target.x += x;
		m_target.y += y;
		m_target.z += z;
		DebugLogRotation();
	}
}

void EditorCamera::CalcUp()
{
	using namespace DirectX;
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.z));
	Vector3 forward;
	forward.x = rot._31;
	forward.y = rot._32;
	forward.z = rot._33;
	forward.Normalize();
	D3DXMATRIX rot2;
	D3DXMatrixRotationYawPitchRoll(&rot2, XMConvertToRadians(m_rotation.y + 90), XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.z));
	Vector3 right;
	right.x = rot2._31;
	right.y = rot2._32;
	right.z = rot2._33;
	right.Normalize();
	m_up = Vector3::Cross(forward, right);
}

const float& EditorCamera::GetDistance()
{
	return m_distance;
}

D3DXMATRIX EditorCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

D3DXMATRIX EditorCamera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

void EditorCamera::DebugLogRotation()
{
	std::string str;
	str = "rx:" + std::to_string(m_rotation.x) + " ry:" + std::to_string(m_rotation.y) + " rz:" + std::to_string(m_rotation.z) + "\n";
	OutputDebugString(str.c_str());
}

void EditorCamera::DebugLogPosition()
{
	std::string str;
	str = "rx:" + std::to_string(m_position.x) + " ry:" + std::to_string(m_position.y) + " rz:" + std::to_string(m_position.z) + "\n";
	OutputDebugString(str.c_str());
}