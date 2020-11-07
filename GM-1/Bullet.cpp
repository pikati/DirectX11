#include "Bullet.h"
#include "Player.h"
#include "manager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "FPS.h"

void Bullet::Initialize()
{
	m_player = CManager::GetScene()->Find("Player");
	m_direction = m_player->GetForward();
}

void Bullet::Update()
{
	gameObject->transform->position += m_direction * m_speed * FPS::deltaTime;
	m_lifeTime--;
	if (m_lifeTime <= 0)
	{
		gameObject->Destroy();
	}
}

void Bullet::OnCollisionEnter(GameObject* obj)
{
	if (obj->tag != "Player")
	{
		gameObject->Destroy();
	}
}