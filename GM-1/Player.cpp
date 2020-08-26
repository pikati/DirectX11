#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "Engine.h"
#include "Player.h"
#include "Transform.h"
#include "AABB.h"
#include "FPS.h"
#include "Animation.h"
#include "ObjectPooler.h"

enum AnimationName
{
	WALK,
	SLASH,
	STAB,
	SHOOT,
	DAMAGE,
	IDOL,
	END
};

Player::Player()
{
	m_collider = nullptr;
	m_animation = nullptr;
	m_velocity = Vector3::zero;
}

Player::~Player()
{

}


void Player::Initialize()
{
	gameObject->transform->rotation = Vector3(0.0f, 0.0f, 0.0f);
	gameObject->transform->scale = Vector3(1.0f, 1.0f, 1.0f);
	ObjectPooler::LoadGameObject("Asset/Prefabs/Bullet.prefab");

	m_collider = gameObject->GetComponent<AABB>();

	m_animation = gameObject->GetComponent<Animation>();
	m_animation->SetSpeed(WALK, 2);
}

void Player::Update()
{
	Vector3 forward = gameObject->GetForward();


	if (m_collider->IsCollision())
	{
		if (m_collider->GetHitGameObject()->tag == "Ground")
		{
			m_isGrounded = true;
		}
	}
	if (m_collider->GetExittGameObject() != nullptr)
	{
		if (m_collider->GetExittGameObject()->tag == "Ground")
		{
			m_isGrounded = false;
		}
	}

	if (CInput::GetKeyTrigger('K'))
	{
		Jump();

	}
	if (CInput::GetKeyTrigger('L'))
	{
		Shot();
	}
	Move();

	if (m_isGrounded)
	{
		m_velocity.y = 0;
	}
	gameObject->transform->position += m_velocity;
	if (gameObject->transform->position.y < -20)
	{
		gameObject->transform->position.y = 2.0f;
		m_velocity.y = 0;
	}
}

void Player::Draw()
{
}

void Player::Finalize()
{
}

void Player::Move()
{
	m_velocity.x = 0;
	m_velocity.z = 0;
	
	if (CInput::GetKeyPress('A'))
	{
		m_velocity.x += -SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 270.0f;
	}
	if (CInput::GetKeyPress('D'))
	{
		m_velocity.x += SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 90.0f;
	}
	if (CInput::GetKeyPress('W'))
	{
		m_velocity.z += SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 0.0f;
	}
	if (CInput::GetKeyPress('S'))
	{
		m_velocity.z += -SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 180.0f;
	}

	m_velocity.y += -m_gravity * FPS::deltaTime * 0.25f;

	if (m_velocity.x != 0 || m_velocity.z != 0)
	{
		m_animation->SetState(WALK);
	}
	else
	{
		m_animation->SetState(IDOL);
	}
}

void Player::Shot()
{
	GameObject* obj;
	obj = ObjectPooler::CreatePrefab("Bullet");
	obj->transform->position = gameObject->transform->position;
	m_animation->SetState(SHOOT);

}

void Player::Jump()
{
	if (m_isGrounded)
	{
		m_velocity.y += 0.5f;
		m_isGrounded = false;
	}
}