#include "Plane.h"
#include "renderer.h"
#include "main.h"
#include "GameObject.h"
#include "Transform.h"
#include "Engine.h"
#include "Texture.h"
#include "LevelLoader.h"
#include "ObjectPooler.h"
#include <DirectXMath.h>
#include <math.h>

void Plane::Initialize()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, 0.0f, -1.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	CalculateNormal(Vector3(vertex[0].Position.x, vertex[0].Position.y, vertex[0].Position.z),
		Vector3(vertex[1].Position.x, vertex[1].Position.y, vertex[1].Position.z),
		Vector3(vertex[2].Position.x, vertex[2].Position.y, vertex[2].Position.z));

	SetCenter(Vector3(vertex[0].Position.x, vertex[0].Position.y, vertex[0].Position.z),
		Vector3(vertex[1].Position.x, vertex[1].Position.y, vertex[1].Position.z),
		Vector3(vertex[2].Position.x, vertex[2].Position.y, vertex[2].Position.z),
		Vector3(vertex[3].Position.x, vertex[3].Position.y, vertex[3].Position.z));

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
	Texture* tex = ObjectPooler::GetTexture(m_textureName);
	if (tex == nullptr)
	{
		tex = new Texture();
		tex->SetTextureName(m_textureName);
		tex->Initialize();
		m_tex = tex->GetTexture();
	}
}

void Plane::Draw()
{
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
	CRenderer::SetShader(SHADER_TYPE::Default);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	if (m_tex != nullptr)
	{
		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_tex);
	}

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->Draw(4, 0);
	//DrawNormal();
}

void Plane::Finalize()
{
	SAFE_RELEASE(m_tex);
}

void Plane::SetTextureName(std::string name)
{
	m_textureName = name;
}

void Plane::CalculateNormal(Vector3 a, Vector3 b, Vector3 c)
{
	using namespace DirectX;
	float x = XMConvertToRadians(gameObject->transform->rotation.x);
	float y = XMConvertToRadians(gameObject->transform->rotation.y);
	float z = XMConvertToRadians(gameObject->transform->rotation.z);
	a = Vector3(a.x, a.y * sinf(x) - a.z * cosf(x), a.y * sinf(x) + a.z * cosf(x));
	a = Vector3(a.x * cos(y) + a.z * sin(y), a.y, -a.x * sinf(y) + a.z * cosf(y));
	a = Vector3(a.x * cos(z) - a.y * sin(z), a.x * sinf(z) + a.y * cosf(z), a.z);
	b = Vector3(b.x, b.y * sinf(x) - b.z * cosf(x), b.y * sinf(x) + b.z * cosf(x));
	b = Vector3(b.x * cos(y) + b.z * sin(y), b.y, -b.x * sinf(y) + b.z * cosf(y));
	b = Vector3(b.x * cos(z) - b.y * sin(z), b.x * sinf(z) + b.y * cosf(z), b.z);
	c = Vector3(c.x, c.y * sinf(x) - c.z * cosf(x), c.y * sinf(x) + c.z * cosf(x));
	c = Vector3(c.x * cos(y) + c.z * sin(y), c.y, -c.x * sinf(y) + c.z * cosf(y));
	c = Vector3(c.x * cos(z) - c.y * sin(z), c.x * sinf(z) + c.y * cosf(z), c.z);
	
	m_meshNormal = Vector3::Cross(b - a, c - a);
	m_meshNormal.Normalize();
}

void Plane::SetCenter(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
{
	m_center.x = (a.x + b.x + c.x + d.x) / 4.0f;
	m_center.y = (a.y + b.y + c.y + d.y) / 4.0f;
	m_center.z = (a.z + b.z + c.z + d.z) / 4.0f;
}

void Plane::DrawNormal()
{
	VERTEX_3D vertex[2];
	ID3D11Buffer* b;
	vertex[0].Position = D3DXVECTOR3(m_center.x, m_center.y, m_center.z);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(m_center.x + m_meshNormal.x, 
									 m_center.y + m_meshNormal.y,
									 m_center.z + m_meshNormal.z);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 2;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &b);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &b, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	CRenderer::GetDeviceContext()->Draw(2, 0);
}

void Plane::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetString(inProp, "fileName", m_textureName);
	JsonHelper::GetInt(inProp, "id", m_id);
}

void Plane::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddString(alloc, inProp, "fileName", m_textureName);
	JsonHelper::AddInt(alloc, inProp, "id", m_id);
}