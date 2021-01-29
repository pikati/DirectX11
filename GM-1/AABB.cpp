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
	m_max.Set(-10000, 10000, 10000);
	m_min.Set(10000, 10000, 10000);
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

	m_pos.Set(0, 0, 0);
	for (int i = 0; i < 14; i++)
	{
		m_v[i].color = Vector4(0, 0, 255, 255);
	}
	m_v[0].pos = m_max;
	m_v[1].pos = Vector3(m_min.x, m_max.y, m_max.z);
	m_v[2].pos = Vector3(m_min.x, m_min.y, m_max.z);
	m_v[3].pos = Vector3(m_max.x, m_min.y, m_max.z);
	m_v[4].pos = Vector3(m_max.x, m_max.y, m_min.z);
	m_v[5].pos = Vector3(m_min.x, m_max.y, m_min.z);
	m_v[6].pos = m_min;
	m_v[7].pos = Vector3(m_max.x, m_min.y, m_min.z);
	m_colliderType = Aabb;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 5;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	VERTEX_3D v[5];
	v[0].Position = D3DXVECTOR3(m_v[0].pos.x, m_v[0].pos.y, m_v[0].pos.z);
	v[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	v[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	v[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	v[1].Position = D3DXVECTOR3(m_v[1].pos.x, m_v[1].pos.y, m_v[1].pos.z);
	v[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	v[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	v[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	v[2].Position = D3DXVECTOR3(m_v[2].pos.x, m_v[2].pos.y, m_v[2].pos.z);
	v[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	v[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	v[2].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	v[3].Position = D3DXVECTOR3(m_v[3].pos.x, m_v[3].pos.y, m_v[3].pos.z);
	v[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	v[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	v[3].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	v[4].Position = D3DXVECTOR3(m_v[3].pos.x, m_v[3].pos.y, m_v[3].pos.z);
	v[4].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	v[4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	v[4].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &v;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vb1);




	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DYNAMIC;
	bd2.ByteWidth = sizeof(VERTEX_3D) * 2;
	bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd2.MiscFlags = 0;
	bd2.StructureByteStride = 0;

	VERTEX_3D vv[2];
	vv[0].Position = D3DXVECTOR3(m_v[8].pos.x, m_v[8].pos.y, m_v[8].pos.z);
	vv[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vv[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vv[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vv[1].Position = D3DXVECTOR3(m_v[9].pos.x, m_v[9].pos.y, m_v[9].pos.z);
	vv[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vv[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vv[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	D3D11_SUBRESOURCE_DATA sd2;
	ZeroMemory(&sd2, sizeof(sd2));
	sd2.pSysMem = &vv;
	sd2.SysMemPitch = 0;
	sd2.SysMemSlicePitch = 0;

	CRenderer::GetDevice()->CreateBuffer(&bd2, &sd2, &m_vb2);

	m_colliderID = SetCollider(this);
}

void AABB::Update()
{
	//hitObject = nullptr;
	m_pos = gameObject->transform->position;
	m_isCollisionThisFrame = false;
	Collider::Update();
}

//“–‚½‚è”»’è•`‰æ—p
void AABB::Draw()
{
	if (!m_isDraw) return;
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
	world._41 = m_pos.x;
	world._42 = m_pos.y;
	world._43 = m_pos.z;
	world._14 = 0;
	world._24 = 0;
	world._34 = 0;
	world._44 = 1.0f;
	CRenderer::SetWorldMatrix(&world);


	ID3D11ShaderResourceView* t = m_texture->GetTexture();


	VERTEX_3D vertex[5];

	vertex[0].Position = D3DXVECTOR3(m_v[0].pos.x, m_v[0].pos.y, m_v[0].pos.z);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(m_v[1].pos.x, m_v[1].pos.y, m_v[1].pos.z);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(m_v[2].pos.x, m_v[2].pos.y, m_v[2].pos.z);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[3].Position = D3DXVECTOR3(m_v[3].pos.x, m_v[3].pos.y, m_v[3].pos.z);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[4].Position = D3DXVECTOR3(m_v[0].pos.x, m_v[0].pos.y, m_v[0].pos.z);
	vertex[4].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[4].TexCoord = D3DXVECTOR2(0.0f, 0.0f);


	D3D11_MAPPED_SUBRESOURCE pdata;
	CRenderer::GetDeviceContext()->Map(m_vb1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertex), sizeof(VERTEX_3D) * 5);
	CRenderer::GetDeviceContext()->Unmap(m_vb1, 0);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vb1, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &t);

	CRenderer::GetDeviceContext()->Draw(5, 0);

	vertex[0].Position = D3DXVECTOR3(m_v[4].pos.x, m_v[4].pos.y, m_v[4].pos.z);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(m_v[5].pos.x, m_v[5].pos.y, m_v[5].pos.z);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(m_v[6].pos.x, m_v[6].pos.y, m_v[6].pos.z);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[3].Position = D3DXVECTOR3(m_v[7].pos.x, m_v[7].pos.y, m_v[7].pos.z);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[4].Position = D3DXVECTOR3(m_v[4].pos.x, m_v[4].pos.y, m_v[4].pos.z);
	vertex[4].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[4].TexCoord = D3DXVECTOR2(0.0f, 0.0f);


	CRenderer::GetDeviceContext()->Map(m_vb1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertex), sizeof(VERTEX_3D) * 5);
	CRenderer::GetDeviceContext()->Unmap(m_vb1, 0);

	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vb1, &stride, &offset);

	CRenderer::SetMaterial(material);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &t);

	CRenderer::GetDeviceContext()->Draw(5, 0);





	VERTEX_3D vertex2[2];
	Vector3 pos = gameObject->transform->position;
	vertex2[0].Position = D3DXVECTOR3(m_v[0].pos.x, m_v[0].pos.y, m_v[0].pos.z);
	vertex2[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex2[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex2[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex2[1].Position = D3DXVECTOR3(m_v[3].pos.x, m_v[4].pos.y, m_v[4].pos.z);
	vertex2[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex2[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex2[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	CRenderer::GetDeviceContext()->Map(m_vb2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertex2), sizeof(VERTEX_3D) * 2);
	CRenderer::GetDeviceContext()->Unmap(m_vb2, 0);

	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vb2, &stride, &offset);

	CRenderer::SetMaterial(material);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &t);

	CRenderer::GetDeviceContext()->Draw(2, 0);


	vertex2[0].Position = D3DXVECTOR3(m_v[1].pos.x, m_v[1].pos.y, m_v[1].pos.z);
	vertex2[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex2[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex2[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex2[1].Position = D3DXVECTOR3(m_v[2].pos.x, m_v[5].pos.y, m_v[5].pos.z);
	vertex2[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex2[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex2[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	CRenderer::GetDeviceContext()->Map(m_vb2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertex2), sizeof(VERTEX_3D) * 2);
	CRenderer::GetDeviceContext()->Unmap(m_vb2, 0);

	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vb2, &stride, &offset);

	CRenderer::SetMaterial(material);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &t);

	CRenderer::GetDeviceContext()->Draw(2, 0);


	vertex2[0].Position = D3DXVECTOR3(m_v[4].pos.x, m_v[2].pos.y, m_v[2].pos.z);
	vertex2[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex2[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex2[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex2[1].Position = D3DXVECTOR3(m_v[7].pos.x, m_v[6].pos.y, m_v[6].pos.z);
	vertex2[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex2[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex2[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	CRenderer::GetDeviceContext()->Map(m_vb2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertex2), sizeof(VERTEX_3D) * 2);
	CRenderer::GetDeviceContext()->Unmap(m_vb2, 0);

	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vb2, &stride, &offset);

	CRenderer::SetMaterial(material);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &t);

	CRenderer::GetDeviceContext()->Draw(2, 0);


	vertex2[0].Position = D3DXVECTOR3(m_v[5].pos.x, m_v[3].pos.y, m_v[3].pos.z);
	vertex2[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex2[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex2[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex2[1].Position = D3DXVECTOR3(m_v[6].pos.x, m_v[7].pos.y, m_v[7].pos.z);
	vertex2[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex2[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex2[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	CRenderer::GetDeviceContext()->Map(m_vb2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertex2), sizeof(VERTEX_3D) * 2);
	CRenderer::GetDeviceContext()->Unmap(m_vb2, 0);

	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vb2, &stride, &offset);

	CRenderer::SetMaterial(material);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &t);

	CRenderer::GetDeviceContext()->Draw(2, 0);
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