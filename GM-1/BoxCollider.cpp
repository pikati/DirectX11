#include "BoxCollider.h"
#include "Transform.h"
#include "LevelLoader.h"
#include "rapidjson/document.h"
#include "main.h"
#include "renderer.h"
#include <DirectXMath.h>
#include "imGui/imgui.h"

BoxCollider::BoxCollider()
{
	m_colliderType = Box;
	if (m_transform == nullptr)
	{
		m_transform = new Transform();
		m_sortingOrder = 0;
	}
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Initialize()
{
	m_colliderID = SetCollider(this);
}

void BoxCollider::Update()
{
	m_isCollisionThisFrame = false;
	Collider::Update();
}

void BoxCollider::Draw()
{
	if (!m_isDraw) return;
	VERTEX_LINE v[8];
	Vector3 pos = gameObject->transform->position;
	v[0].Position = Vector3(m_min.x, m_max.y, m_min.z);
	v[1].Position = Vector3(m_max.x, m_max.y, m_min.z);
	v[2].Position = Vector3(m_min.x, m_min.y, m_min.z);
	v[3].Position = Vector3(m_max.x, m_min.y, m_min.z);
	v[4].Position = Vector3(m_min.x, m_max.y, m_max.z);
	v[5].Position = Vector3(m_max.x, m_max.y, m_max.z);
	v[6].Position = Vector3(m_min.x, m_min.y, m_max.z);
	v[7].Position = Vector3(m_max.x, m_min.y, m_max.z);

	int* index = new int[24];
	index[0] = 0;
	index[1] = 1;
	index[2] = 0;
	index[3] = 2;
	index[4] = 1;
	index[5] = 3;
	index[6] = 2;
	index[7] = 3;
	index[8] = 0;
	index[9] = 4;
	index[10] = 1;
	index[11] = 5;
	index[12] = 2;
	index[13] = 6;
	index[14] = 3;
	index[15] = 7;
	index[16] = 4;
	index[17] = 5;
	index[18] = 4;
	index[19] = 6;
	index[20] = 5;
	index[21] = 7;
	index[22] = 6;
	index[23] = 7;

	ID3D11Buffer* pVB;
	ID3D11Buffer* pIB;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_LINE) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = v;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pVB);

	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(unsigned int) * 24;
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;
	bd2.MiscFlags = 0;
	bd2.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd2;
	ZeroMemory(&sd2, sizeof(sd2));
	sd2.pSysMem = index;
	sd2.SysMemPitch = 0;
	sd2.SysMemSlicePitch = 0;

	CRenderer::GetDevice()->CreateBuffer(&bd2, &sd2, &pIB);

	using namespace DirectX;
	D3DXMATRIX world, rotation;
	D3DXMatrixRotationYawPitchRoll(&rotation, XMConvertToRadians(gameObject->transform->rotation.y), XMConvertToRadians(gameObject->transform->rotation.x), XMConvertToRadians(gameObject->transform->rotation.z));
	world._11 = gameObject->transform->scale.x * rotation._11;
	world._12 = gameObject->transform->scale.x * rotation._12;
	world._13 = gameObject->transform->scale.x * rotation._13;
	world._21 = gameObject->transform->scale.y * rotation._21;
	world._22 = gameObject->transform->scale.y * rotation._22;
	world._23 = gameObject->transform->scale.y * rotation._23;
	world._31 = gameObject->transform->scale.z * rotation._31;
	world._32 = gameObject->transform->scale.z * rotation._32;
	world._33 = gameObject->transform->scale.z * rotation._33;
	world._41 = gameObject->transform->position.x;
	world._42 = gameObject->transform->position.y;
	world._43 = gameObject->transform->position.z;
	world._14 = 0;
	world._24 = 0;
	world._34 = 0;
	world._44 = 1.0f;
	CRenderer::SetWorldMatrix(&world);

	CRenderer::SetVertexShader(SHADER_TYPE::Color);
	CRenderer::SetPixelShader(SHADER_TYPE::Color);
	CRenderer::SetInputLayout(SHADER_TYPE::Color);

	UINT stride = sizeof(VERTEX_LINE);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	// インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(24, 0, 0);

	CRenderer::SetShader(SHADER_TYPE::Default);

	SAFE_RELEASE(pVB);
	SAFE_RELEASE(pIB);
}

void BoxCollider::Finalize()
{
	DeleteCollider(m_colliderID);
	delete m_transform;
}

Vector3 BoxCollider::GetPosition()
{
	return (m_min + m_max) * 0.5 + gameObject->transform->position;
}

void BoxCollider::SetMax(Vector3 max)
{
	m_max = max;
}

void BoxCollider::SetMin(Vector3 min)
{
	m_min = min;
}

float BoxCollider::GetLength(int i)
{
	if (i == 0)
	{
		return fabs(m_max.x - m_min.x) / 2;
	}
	else if (i == 1)
	{
		return fabs(m_max.y - m_min.y) / 2;
	}
	else if (i == 2)
	{
		return fabs(m_max.z - m_min.z) / 2;
	}
	return -1;
}

Vector3 BoxCollider::GetAxis(int i)
{
	D3DXMATRIX r, s, world;
	Vector3 rotation = gameObject->transform->rotation;
	Vector3 scale = gameObject->transform->scale;
	D3DXMatrixRotationYawPitchRoll(&r, rotation.y, rotation.x, rotation.z);
	D3DXMatrixScaling(&s, scale.x, scale.y, scale.z);
	world = s * r;
	Vector3 v;

	if (i == 0)
	{
		v.x = world._11;
		v.y = world._12;
		v.z = world._13;
	}
	if (i == 1)
	{
		v.x = world._21;
		v.y = world._22;
		v.z = world._23;
	}
	if (i == 2)
	{
		v.x = world._31;
		v.y = world._32;
		v.z = world._33;
	}
	return v;

}

void BoxCollider::DrawInformation()
{
	std::string name = typeid(*this).name();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(name.substr(6).c_str());

	float max[3] = { m_max.x, m_max.y, m_max.z };
	float min[3] = { m_min.x, m_min.y, m_min.z };
	ImGui::InputFloat3("maxPosition", max);
	ImGui::InputFloat3("minPosition", min);
	//ImGui::Checkbox("isKinematic", &m_isKinematic);
	ImGui::Checkbox("DrawCollider", &m_isDraw);
	m_max = { max[0], max[1], max[2] };
	m_min = { min[0], min[1], min[2] };

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void BoxCollider::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetVector3(inProp, "max", m_max);
	JsonHelper::GetVector3(inProp, "min", m_min);
	JsonHelper::GetVector3(inProp, "position", m_transform->position);
	JsonHelper::GetVector3(inProp, "rotation", m_transform->rotation);
	JsonHelper::GetVector3(inProp, "scale", m_transform->scale);
}

void BoxCollider::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddVector3(alloc, inProp, "max", m_max);
	JsonHelper::AddVector3(alloc, inProp, "min", m_min);
	JsonHelper::AddVector3(alloc, inProp, "position", m_transform->position);
	JsonHelper::AddVector3(alloc, inProp, "rotation", m_transform->rotation);
	JsonHelper::AddVector3(alloc, inProp, "scale", m_transform->scale);
}