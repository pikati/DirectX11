#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "Engine.h"
#include "model.h"
#include "Bullet.h"
#include "Transform.h"
#include "Scene.h"
#include "manager.h"
#include "player.h"
#include "Camera.h"
#include "SphereCollider.h"
#include <DirectXMath.h>
#include "SpriteEffect.h"

static Scene* g_scene;
static GameObject* g_player;
static GameObject* g_camera;
static GameObject* g_effect;

Bullet::Bullet()
{
	m_model = nullptr;
	m_collider = nullptr;
}

Bullet::~Bullet()
{

}


void Bullet::Initialize()
{
	m_model = new CModel();
	m_model->Load("Asset\\Model\\torus.obj");

	g_scene = CManager::GetScene();
	g_player = g_scene->Find("Player");
	g_camera = g_scene->Find("MainCamera");

	gameObject->transform->position = g_player->transform->position;
	gameObject->transform->rotation = Vector3::zero;
	gameObject->transform->scale = Vector3::one;
	m_collider = gameObject->GetComponent<SphereCollider>();
	m_collider->SetCenter(Vector3::zero);
	m_collider->SetRadius(1.0f);

	g_effect = new GameObject();
	g_effect->AddComponent<SpriteEffect>();
	g_effect->Initialize();
}

void Bullet::Update()
{
	gameObject->transform->position += Vector3(0.0f, 0.0f, 0.1f);
	if (gameObject->transform->position.z >= 10.0f)
	{
		gameObject->Destroy();
	}
	if (m_collider->IsCollision())
	{
		std::string tag = m_collider->GetHitGameObject()->tag;
		if (tag == "Enemy")
		{
			gameObject->Destroy();
			g_scene->CreatePrefab(g_effect);
		}
	}
}

void Bullet::Draw()
{
	//ビューマトリクス設定
	D3DXMATRIX viewMatrix;
	Vector3 target = dynamic_cast<Camera*>(g_camera->GetComponent("Camera"))->GetTarget();
	D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(g_camera->transform->position.x, g_camera->transform->position.y, g_camera->transform->position.z), &D3DXVECTOR3(target.x, target.y, target.z), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CRenderer::SetViewMatrix(&viewMatrix);

	//プロジェクションマトリクス設定
	D3DXMATRIX projctionMatrix;
	D3DXMatrixPerspectiveFovLH(&projctionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&projctionMatrix);

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

void Bullet::Finalize()
{
	m_model->Unload();
	delete m_model;
}