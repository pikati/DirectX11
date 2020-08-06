#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "Engine.h"
#include "model.h"
#include "Enemy.h"
#include "Transform.h"
#include "SphereCollider.h"
#include <DirectXMath.h>

Enemy::Enemy()
{
	m_model = nullptr;
	m_collider = nullptr;
}

Enemy::~Enemy()
{

}


void Enemy::Initialize()
{
	m_model = new CModel();
	m_model->Load("Asset\\Models\\Ghost.obj");

	gameObject->transform->rotation = Vector3(0.0f, 100.0f, 0.0f);
	gameObject->transform->scale = Vector3(0.6f, 0.6f, 0.6f);

	m_collider = gameObject->GetComponent<SphereCollider>();
	m_collider->SetCenter(Vector3::zero);
	m_collider->SetRadius(0.5f);
}

void Enemy::Update()
{
	if (m_collider->IsCollision())
	{
		gameObject->Destroy();
	}
}

void Enemy::Draw()
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
	m_model->Draw();
}

void Enemy::Finalize()
{
	m_model->Unload();
	delete m_model;
}