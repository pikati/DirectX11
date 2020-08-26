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
#include "FPS.h"



Bullet::Bullet()
{
	m_collider = nullptr;
	m_direction = Vector3::zero;
	m_scene = nullptr;
	m_player = nullptr;
	m_camera = nullptr;
	m_effect = nullptr;
}

Bullet::~Bullet()
{

}


void Bullet::Initialize()
{
	m_scene = CManager::GetScene();
	m_player = m_scene->Find("Player");
	m_camera = m_scene->Find("MainCamera");

	gameObject->transform->position = m_player->transform->position;
	gameObject->transform->rotation = Vector3::zero;
	gameObject->transform->scale = Vector3::one;
	m_collider = gameObject->GetComponent<SphereCollider>();
	m_collider->SetCenter(Vector3::zero);
	m_collider->SetRadius(1.0f);

	m_effect = new GameObject();
	m_effect->AddComponent<SpriteEffect>();
	m_effect->Initialize();
	float rotation = m_player->transform->rotation.y;
	if (rotation == 0)
	{
		m_direction.z = SPEED;
	}
	else if (rotation == 90.0f)
	{
		m_direction.x = SPEED;
	}
	else if (rotation == 180.0f)
	{
		m_direction.z = -SPEED;
	}
	else
	{
		m_direction.x = -SPEED;
	}
}

void Bullet::Update()
{
	if (m_frame == 60)
	{
		gameObject->Destroy();
		return;
	}
	gameObject->transform->position += m_direction * FPS::deltaTime;
	if (m_collider->IsCollision())
	{
		GameObject* obj = m_collider->GetHitGameObject();
		if (obj != nullptr)
		{
			std::string tag = obj->tag;
			if (tag == "Enemy")
			{
				gameObject->Destroy();
				GameObject* g = m_scene->CreatePrefab(m_effect);
				g->transform->position.Set(gameObject->transform->position.x, gameObject->transform->position.y, gameObject->transform->position.z);
				return;
			}
		}
	}
	m_frame++;
}

void Bullet::Draw()
{
}

void Bullet::Finalize()
{

}