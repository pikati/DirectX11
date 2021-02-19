#include "AABB.h"
#include "GameObject.h"
#include "Transform.h"
#include "renderer.h"
#include "Texture.h"
#include "LevelLoader.h"
#include "rapidjson/document.h"
#include <DirectXMath.h>
#include "imGui/imgui.h"
#include "Inspector.h"

AABB::AABB()
{
	m_max.Set(1, 1, 1);
	m_min.Set(-1, -1, -1);
	m_texture = new Texture();
	m_texture->SetTextureName("Asset/Texture/fade.png");
	m_texture->Initialize();
}

AABB::~AABB()
{
	SAFE_DELETE(m_texture);
}

void AABB::SystemInitialize()
{
	m_colliderID = SetCollider(this);
	m_colliderType = Aabb;
}

void AABB::Update()
{
	//hitObject = nullptr;
	m_pos = gameObject->transform->position;
	m_isCollisionThisFrame = false;
	Collider::Update();
}

//当たり判定描画用
void AABB::Draw()
{
	if (!m_isDraw) return;
	VERTEX_LINE v[8];
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
	D3DXMatrixRotationYawPitchRoll(&rotation, XMConvertToRadians(0), XMConvertToRadians(0), XMConvertToRadians(0));
	world._11 = 1 * rotation._11;
	world._12 = 1 * rotation._12;
	world._13 = 1 * rotation._13;
	world._21 = 1 * rotation._21;
	world._22 = 1 * rotation._22;
	world._23 = 1 * rotation._23;
	world._31 = 1 * rotation._31;
	world._32 = 1 * rotation._32;
	world._33 = 1 * rotation._33;
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

void AABB::SystemFinalize()
{
	DeleteCollider(m_colliderID);
}

void AABB::SetMax(Vector3 max)
{
	m_max = max;
	m_v[0].pos = m_max;
	m_v[1].pos = Vector3(m_min.x, m_max.y, m_max.z);
	m_v[2].pos = Vector3(m_min.x, m_min.y, m_max.z);
	m_v[3].pos = Vector3(m_max.x, m_min.y, m_max.z);
	m_v[4].pos = Vector3(m_max.x, m_max.y, m_min.z);
	m_v[5].pos = Vector3(m_min.x, m_max.y, m_min.z);
	m_v[6].pos = m_min;
	m_v[7].pos = Vector3(m_max.x, m_min.y, m_min.z);
}

void AABB::SetMin(Vector3 min)
{
	m_min = min;
	m_v[0].pos = m_max;
	m_v[1].pos = Vector3(m_min.x, m_max.y, m_max.z);
	m_v[2].pos = Vector3(m_min.x, m_min.y, m_max.z);
	m_v[3].pos = Vector3(m_max.x, m_min.y, m_max.z);
	m_v[4].pos = Vector3(m_max.x, m_max.y, m_min.z);
	m_v[5].pos = Vector3(m_min.x, m_max.y, m_min.z);
	m_v[6].pos = m_min;
	m_v[7].pos = Vector3(m_max.x, m_min.y, m_min.z);
}

void AABB::SetMax(float x, float y, float z)
{
	m_max.Set(x, y, z);
	m_v[0].pos = m_max;
	m_v[1].pos = Vector3(m_min.x, m_max.y, m_max.z);
	m_v[2].pos = Vector3(m_min.x, m_min.y, m_max.z);
	m_v[3].pos = Vector3(m_max.x, m_min.y, m_max.z);
	m_v[4].pos = Vector3(m_max.x, m_max.y, m_min.z);
	m_v[5].pos = Vector3(m_min.x, m_max.y, m_min.z);
	m_v[6].pos = m_min;
	m_v[7].pos = Vector3(m_max.x, m_min.y, m_min.z);
}

void AABB::SetMin(float x, float y, float z)
{
	m_min.Set(x, y, z);
	m_v[0].pos = m_max;
	m_v[1].pos = Vector3(m_min.x, m_max.y, m_max.z);
	m_v[2].pos = Vector3(m_min.x, m_min.y, m_max.z);
	m_v[3].pos = Vector3(m_max.x, m_min.y, m_max.z);
	m_v[4].pos = Vector3(m_max.x, m_max.y, m_min.z);
	m_v[5].pos = Vector3(m_min.x, m_max.y, m_min.z);
	m_v[6].pos = m_min;
	m_v[7].pos = Vector3(m_max.x, m_min.y, m_min.z);
}

void AABB::SetPosition(Vector3 pos)
{
	m_pos = pos;
}

Vector3 AABB::GetMax()
{
	return m_max;
}

Vector3 AABB::GetMin()
{
	return m_min;
}

Vector3 AABB::GetPosition()
{
	return m_pos;
}

float AABB::GetHeight()
{
	return m_max.y - m_min.y;
}

void AABB::DrawInformation()
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
	ImGui::Checkbox("DrawCollider", &m_isDraw);
	m_max = { max[0], max[1], max[2] };
	m_min = { min[0], min[1], min[2] };
	if (ImGui::Button("Delete"))
	{
		this->SystemFinalize();
		gameObject->DeleteComponent<AABB>();
		Inspector::DeleteInformation();
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void AABB::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetVector3(inProp, "max", m_max);
	JsonHelper::GetVector3(inProp, "min", m_min);
}

void AABB::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddVector3(alloc, inProp, "max", m_max);
	JsonHelper::AddVector3(alloc, inProp, "min", m_min);
}