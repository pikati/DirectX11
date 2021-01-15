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
#include "Editor.h"
#include "imgui/imgui.h"

static D3DMATRIX m;
static D3DXMATRIX w;

Camera::Camera()
{
	
}

Camera::~Camera()
{

}

void Camera::Initialize()
{
	m_target = Vector3(0.0f, 0.0f, 0.0f);
	m_viewPort.Width = static_cast<FLOAT>(m_viewPortWidth);
	m_viewPort.Height = static_cast<FLOAT>(m_viewPortHeight);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = static_cast<FLOAT>(m_viewPortTopLeftX);
	m_viewPort.TopLeftY = static_cast<FLOAT>(m_viewPortTopLeftY);
	CManager::GetScene()->AddRenderNum();
}

void Camera::Update()
{
	CRenderer::SetCameraPosition(gameObject->transform->position);
}

void Camera::Draw()
{
	if (!Editor::IsPlay()) return;
	if (m_renderNum != CManager::GetScene()->GetRenderNum()) return;
	CRenderer::SetViewPort(&m_viewPort);
	if (m_renderNum == 0)
	{
		//�r���[�}�g���N�X�ݒ�
		D3DXMatrixLookAtLH(&m_viewMatrix, &D3DXVECTOR3(gameObject->transform->position.x, gameObject->transform->position.y, gameObject->transform->position.z), &D3DXVECTOR3(m_target.x, m_target.y, m_target.z), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		CRenderer::SetViewMatrix(&m_viewMatrix);
		m = m_viewMatrix;
		//�v���W�F�N�V�����}�g���N�X�ݒ�
		D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
		w = m_projectionMatrix;
		CRenderer::SetProjectionMatrix(&m_projectionMatrix);
		CheckView();
	}
	else if (m_renderNum == 1)
	{
		D3DXMatrixLookAtLH(&m_viewMatrix, &D3DXVECTOR3(gameObject->transform->position.x, gameObject->transform->position.y, gameObject->transform->position.z), &D3DXVECTOR3(0.0f, m_target.y, m_target.z), &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		CRenderer::SetViewMatrix(&m_viewMatrix);
		m = m_viewMatrix;
		//�v���W�F�N�V�����}�g���N�X�ݒ�
		D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, 1.0f, (float)SCREEN_WIDTH / 2 / SCREEN_HEIGHT, 1.0f, 1000.0f);
		CRenderer::SetProjectionMatrix(&m_projectionMatrix);
	}
}

void Camera::Finalize()
{
	CManager::GetScene()->SetRenderNum(0);
}

void Camera::CheckView()
{
	std::list<GameObject*>* objects = CManager::GetScene()->GetAllGameObject();
	D3DXMATRIX vp, invvp;
	vp = m_viewMatrix * m_projectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	vpos[0] = { -1.0f,  1.0f, 1.0f };
	vpos[1] = { 1.0f,  1.0f, 1.0f };
	vpos[2] = { -1.0f, -1.0f, 1.0f };
	vpos[3] = { 1.0f, -1.0f, 1.0f };

	//������̉���4���_�̈ʒu���t�s��ŋ��߂�
	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	for (int i = OBJECTS; i < LAYER_MAX; i++)
	{
		for (GameObject* object : objects[i])
		{
			BoundingBox* bb = object->GetBoundingBox();
			if (bb == nullptr) continue;

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

			//4�ʂ̕`�悷�邩�ǂ�������
			bool isDraw[4] = { false, false, false, false };
			for (int j = 0; j < 8; j++)
			{
				//����
				v = pos[j] - mPos;
				v1 = { wpos[0].x - mPos.x, wpos[0].y - mPos.y, wpos[0].z - mPos.z };
				v2 = { wpos[2].x - mPos.x, wpos[2].y - mPos.y, wpos[2].z - mPos.z };
				n = Vector3::Cross(v1, v2);

				if (Vector3::Dot(n, v) > 0.0f)
				{
					isDraw[0] = true;
				}

				//���
				v1 = { wpos[1].x - mPos.x, wpos[1].y - mPos.y, wpos[1].z - mPos.z };
				v2 = { wpos[0].x - mPos.x, wpos[0].y - mPos.y, wpos[0].z - mPos.z };
				n = Vector3::Cross(v1, v2);

				if (Vector3::Dot(n, v) > 0.0f)
				{
					isDraw[1] = true;
				}

				//�E��
				v1 = { wpos[3].x - mPos.x, wpos[3].y - mPos.y, wpos[3].z - mPos.z };
				v2 = { wpos[1].x - mPos.x, wpos[1].y - mPos.y, wpos[1].z - mPos.z };
				n = Vector3::Cross(v1, v2);

				if (Vector3::Dot(n, v) > 0.0f)
				{
					isDraw[2] = true;
				}

				//����
				v1 = { wpos[2].x - mPos.x, wpos[2].y - mPos.y, wpos[2].z - mPos.z };
				v2 = { wpos[3].x - mPos.x, wpos[3].y - mPos.y, wpos[3].z - mPos.z };
				n = Vector3::Cross(v1, v2);

				if (Vector3::Dot(n, v) > 0.0f)
				{
					isDraw[3] = true;
				}
			}
			//�ǂꂩ�̖ʂ̔����8���_���O���Ȃ�`�悵�Ȃ�
			object->IsDraw(isDraw[0] && isDraw[1] && isDraw[2] && isDraw[3]);
		}
	}
}

D3DXMATRIX Camera::GetViewMatrix()
{
	return m;
}

D3DXMATRIX Camera::GetProjectionMatrix()
{
	return w;
}

void Camera::SetLookAt(Vector3 lookPoint)
{
	m_target = lookPoint;
}

void Camera::SetViwePort(D3D11_VIEWPORT viewPort)
{
	m_viewPortHeight = static_cast<int>(viewPort.Height);
	m_viewPortWidth = static_cast<int>(viewPort.Width);
	m_viewPortTopLeftX = static_cast<int>(viewPort.TopLeftX);
	m_viewPortTopLeftY = static_cast<int>(viewPort.TopLeftY);
}

void Camera::DrawInformation()
{
	std::string name = typeid(*this).name();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(name.substr(6).c_str());

	float target[3] = { m_target.x, m_target.y, m_target.z };
	ImGui::InputFloat3("target", target, 1, 10);
	m_target = { target[0], target[1], target[2] };
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
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
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddInt(alloc, inProp, "viewWidth", m_viewPortWidth);
	JsonHelper::AddInt(alloc, inProp, "viewHeight", m_viewPortHeight);
	JsonHelper::AddInt(alloc, inProp, "topLeftX", m_viewPortTopLeftX);
	JsonHelper::AddInt(alloc, inProp, "topLeftY", m_viewPortTopLeftY);
	JsonHelper::AddInt(alloc, inProp, "renderNum", m_renderNum);
	JsonHelper::AddInt(alloc, inProp, "id", m_id);
}