#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
#include "Player.h"
#include "Scene.h"
#include "manager.h"
#include "GameObject.h"
#include "BoundingBox.h"
#include "LevelLoader.h"

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
	m_viewPort.Width = m_viewPortWidth;
	m_viewPort.Height = m_viewPortHeight;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = m_viewPortTopLeftX;
	m_viewPort.TopLeftY = m_viewPortTopLeftY;
}

void Camera::Update()
{
	/*if (!m_inisialized)
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
	gameObject->transform->position.z = m_target.z - cosf(m_rotation) * m_distance;*/

	if (CInput::GetKeyPress('Z'))
	{
		m_target.x += 0.1f;
	}
	if (CInput::GetKeyPress('X'))
	{
		m_target.x += -0.1f;
	}
	if (CInput::GetKeyPress('N'))
	{
		m_target.y += 0.1f;
	}
	if (CInput::GetKeyPress('M'))
	{
		m_target.y += -0.1f;
	}

}

void Camera::Draw()
{
	//ビューマトリクス設定
	D3DXMatrixLookAtLH(&m_viewMatrix, &D3DXVECTOR3(gameObject->transform->position.x, gameObject->transform->position.y, gameObject->transform->position.z), &D3DXVECTOR3(m_target.x, m_target.y, m_target.z), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CRenderer::SetViewMatrix(&m_viewMatrix);
	m = m_viewMatrix;
	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&m_projectionMatrix);
	CheckView();
}

void Camera::CheckView()
{
	std::list<GameObject*>* objects = CManager::GetScene()->GetAllGameObject();
	D3DXMATRIX vp, invvp;
	vp = m_viewMatrix * m_projectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	for (int i = OBJECTS; i < LAYER_MAX; i++)
	{
		for (GameObject* object : objects[i])
		{
			BoundingBox* bb = object->GetBoundingBox();
			if (bb == nullptr) return;

			D3DXVECTOR3 vpos[4];
			D3DXVECTOR3 wpos[4];

			vpos[0] = { -1.0f,  1.0f, 1.0f };
			vpos[1] = { 1.0f,  1.0f, 1.0f };
			vpos[2] = { -1.0f, -1.0f, 1.0f };
			vpos[3] = { 1.0f, -1.0f, 1.0f };

			//視錐台の4頂点の位置を逆行列で求める
			D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
			D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
			D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
			D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

			Vector3 v, v1, v2, n;

			Vector3 mPos = gameObject->transform->position;
			
			Vector3 pos[8];
			Vector3 max = bb->m_max;
			Vector3 min = bb->m_min;
			pos[0] = { max.x, max.y, min.z };
			pos[1] = { min.x, max.y, min.z };
			pos[2] = { max.x, min.y, min.z };
			pos[3] = { min.x, min.y, min.z };
			pos[4] = { max.x, max.y, max.z };
			pos[5] = { min.x, max.y, max.z };
			pos[6] = { max.x, min.y, max.z };
			pos[7] = { min.x, min.y, max.z };

			//左面分
			for (int i = 0; i < 8; i++)
			{
				v = pos[i] - mPos;
				v1 = { wpos[0].x - mPos.x, wpos[0].y - mPos.y, wpos[0].z - mPos.z };
				v2 = { wpos[2].x - mPos.x, wpos[2].y - mPos.y, wpos[2].z - mPos.z };
				n = Vector3::Cross(v1, v2);

				//n.Normalize();

				if (Vector3::Dot(n, v) < 0.0f)
				{
					object->IsDraw(false);
				}

				//上面分

				v1 = { wpos[1].x - mPos.x, wpos[1].y - mPos.y, wpos[1].z - mPos.z };
				v2 = { wpos[0].x - mPos.x, wpos[0].y - mPos.y, wpos[0].z - mPos.z };
				n = Vector3::Cross(v1, v2);

				n.Normalize();

				if (Vector3::Dot(n, v) < 0.0f)
				{
					object->IsDraw(false);
				}

				//右面分
				v1 = { wpos[3].x - mPos.x, wpos[3].y - mPos.y, wpos[3].z - mPos.z };
				v2 = { wpos[1].x - mPos.x, wpos[1].y - mPos.y, wpos[1].z - mPos.z };
				n = Vector3::Cross(v1, v2);

				n.Normalize();

				if (Vector3::Dot(n, v) < 0.0f)
				{
					object->IsDraw(false);
				}

				//下面分
				v1 = { wpos[2].x - mPos.x, wpos[2].y - mPos.y, wpos[2].z - mPos.z };
				v2 = { wpos[3].x - mPos.x, wpos[3].y - mPos.y, wpos[3].z - mPos.z };
				n = Vector3::Cross(v1, v2);

				n.Normalize();

				if (Vector3::Dot(n, v) < 0.0f)
				{
					object->IsDraw(false);
				}
			}
			
		}
	}
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


void Camera::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetInt(inProp, "viewWidth", m_viewPortWidth);
	JsonHelper::GetInt(inProp, "viewHeight", m_viewPortHeight);
	JsonHelper::GetInt(inProp, "topLeftX", m_viewPortTopLeftX);
	JsonHelper::GetInt(inProp, "topLeftY", m_viewPortTopLeftY);
	JsonHelper::GetInt(inProp, "renderNum", m_renderNum);
	JsonHelper::GetInt(inProp, "id", m_id);
}

void Camera::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddInt(alloc, inProp, "viewWidth", m_viewPortWidth);
	JsonHelper::AddInt(alloc, inProp, "viewHeight", m_viewPortHeight);
	JsonHelper::AddInt(alloc, inProp, "topLeftX", m_viewPortTopLeftX);
	JsonHelper::AddInt(alloc, inProp, "topLeftY", m_viewPortTopLeftY);
	JsonHelper::AddInt(alloc, inProp, "renderNum", m_renderNum);
	JsonHelper::AddInt(alloc, inProp, "id", m_id);
}