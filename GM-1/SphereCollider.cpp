#include "SphereCollider.h"
#include "Transform.h"
#include "LevelLoader.h"
#include "rapidjson/document.h"
#include "main.h"
#include "renderer.h"
#include <DirectXMath.h>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"

static const int num = 192;

SphereCollider::SphereCollider()
{
	m_colliderType = Sphere;
	if (m_transform == nullptr)
	{
		m_transform = new Transform();
		m_sortingOrder = 0;
	}
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Initialize()
{
	m_colliderID = SetCollider(this);
}

void SphereCollider::Update()
{
	m_isCollisionThisFrame = false;
	Collider::Update();
	//hitObject = nullptr;
}

void SphereCollider::Draw()
{
	if (!m_isDraw) return;
	VERTEX_LINE v[num];

	float rad = DirectX::XM_2PI / (num / 3);
	float a = 0;
	for (int i = 0; i < num / 3; i++)
	{
		v[i].Position.x = m_radius * cos(a);
		v[i].Position.y = m_radius * sin(a);
		v[i].Position.z = 0;
		v[i].Diffuse = { 0, 0, 1, 1 };
		a += rad;
	}
	a = 0;
	for (int i = num / 3; i < num / 3 * 2; i++)
	{
		v[i].Position.x = 0;
		v[i].Position.y = m_radius * sin(a);
		v[i].Position.z = m_radius * cos(a);
		v[i].Diffuse = { 0, 0, 1, 1 };
		a += rad;
	}
	a = 0;
	for (int i = num / 3 * 2; i < num; i++)
	{
		v[i].Position.x = m_radius * sin(a);
		v[i].Position.y =0 ;
		v[i].Position.z = m_radius * cos(a);
		v[i].Diffuse = { 0, 0, 1, 1 };
		a += rad;
	}

	ID3D11Buffer* pVB;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_LINE) * num;
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

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// ポリゴン描画
	CRenderer::GetDeviceContext()->Draw(num, 0);

	CRenderer::SetShader(SHADER_TYPE::Default);

	SAFE_RELEASE(pVB);
}

void SphereCollider::Finalize()
{
	DeleteCollider(m_colliderID);
	delete m_transform;
}

void SphereCollider::SetCenter(Vector3 center)
{
	m_center = center;
}

void SphereCollider::SetCenter(float x, float y, float z)
{
	m_center.Set(x, y, z);
}

void SphereCollider::SetRadius(float radius)
{
	m_radius = radius;
}

Vector3 SphereCollider::GetPosition()
{
	return m_center + gameObject->transform->position;
}

float SphereCollider::GetRadius()
{
	return m_radius;
}

void SphereCollider::SetProperties(Component* c)
{
	SphereCollider* s = dynamic_cast<SphereCollider*>(c);
	m_center = s->m_center;
	m_radius = s->m_radius;
}

void SphereCollider::SetInspector()
{
	std::string name = typeid(*this).name();
	ImGui::Text(name.substr(6).c_str());
	float center[3] = { m_center.x, m_center.y, m_center.z };
	ImGui::InputFloat3("center", center);
	ImGui::InputFloat("radius", &m_radius, 0.01f, 1.0f, 5);
	//ImGui::Checkbox("isKinematic", &m_isKinematic);
	ImGui::Checkbox("DrawCollider", &m_isDraw);
	m_center = { center[0], center[1], center[2] };
}

void SphereCollider::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetVector3(inProp, "center", m_center);
	JsonHelper::GetFloat(inProp, "radius", m_radius);
	JsonHelper::GetVector3(inProp,  "position", m_transform->position);
	JsonHelper::GetVector3(inProp,  "rotation", m_transform->rotation);
	JsonHelper::GetVector3(inProp,  "scale", m_transform->scale);
}

void SphereCollider::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddVector3(alloc, inProp, "center", m_center);
	JsonHelper::AddFloat(alloc, inProp, "radius", m_radius);
	JsonHelper::AddVector3(alloc, inProp, "position", m_transform->position);
	JsonHelper::AddVector3(alloc, inProp, "rotation", m_transform->rotation);
	JsonHelper::AddVector3(alloc, inProp, "scale", m_transform->scale);
}