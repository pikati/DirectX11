#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "Engine.h"
#include "model.h"
#include "Player.h"
#include "Transform.h"
#include "manager.h"
#include "Scene.h"
#include "SphereCollider.h"
#include "FPS.h"
#include <DirectXMath.h>

static GameObject* g_bullet;
static Scene* g_scene;

Player::Player()
{
	m_model = nullptr;
}

Player::~Player()
{

}


void Player::Initialize()
{
	m_model = new CModel();
	m_model->Load("Asset\\Model\\Girl.obj");

	gameObject->transform->rotation = Vector3(0.0f, 0.0f, 0.0f);
	gameObject->transform->scale    = Vector3(10.0f, 10.0f, 10.0f);
	g_scene = CManager::GetScene();
	g_bullet = g_scene->LoadPrefab("Asset/Prefabs/Bullet.txt");
	/*m_collider = gameObject->GetComponent<SphereCollider>();
	m_collider->SetCenter(Vector3::zero);
	m_collider->SetRadius(0);*/
}

void Player::Update()
{
	Vector3 forward = gameObject->GetForward();
	if (CInput::GetKeyPress('A'))
	{
		//gameObject->transform->position += Vector3(-0.1f, 0.0f, 0.0f);
		gameObject->transform->rotation.y += -180.0f * FPS::deltaTime;
	}
	if (CInput::GetKeyPress('D'))
	{
		//gameObject->transform->position += Vector3(0.1f, 0.0f, 0.0f);
		gameObject->transform->rotation.y += 180.0f * FPS::deltaTime;
	}
	if (CInput::GetKeyPress('W'))
	{
		//gameObject->transform->position += Vector3(0.0f, 0.0f, 0.1f);
		gameObject->transform->position += forward * 10.0f * FPS::deltaTime * -1;
	}
	if (CInput::GetKeyPress('S'))
	{
		//gameObject->transform->position += Vector3(0.0f, 0.0f, -0.1f);
		gameObject->transform->position += forward * 10.0f * FPS::deltaTime;
	}

	if (CInput::GetKeyTrigger('P'))
	{
		g_scene->CreatePrefab(g_bullet);
	}
}

void Player::Draw()
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

void Player::Finalize()
{
	m_model->Unload();
	delete m_model;
}