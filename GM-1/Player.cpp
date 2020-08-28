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
	ANIMATION_END
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

	CheckGrounded();

	if (CInput::GetKeyTrigger('K'))
	{
		Jump();

	}
	//if (CInput::GetKeyTrigger('L'))
	//{
	//	Shot();
	//}
	//if (CInput::GetKeyPress('J'))
	//{
	//	//Slash();
	//	gameObject->transform->position.y += 1.0f * FPS::deltaTime;
	//}if (CInput::GetKeyPress('I'))
	//{
	//	//Slash();
	//	gameObject->transform->position.y += -1.0f * FPS::deltaTime;
	//}
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
		dir = LEFT;
	}
	if (CInput::GetKeyPress('D'))
	{
		m_velocity.x += SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 90.0f;
		dir = RIGHT;
	}
	if (CInput::GetKeyPress('W'))
	{
		m_velocity.z += SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 0.0f;
		dir = FORWARD;
	}
	if (CInput::GetKeyPress('S'))
	{
		m_velocity.z += -SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 180.0f;
		dir = BACK;
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

void Player::Slash()
{
	m_animation->SetState(SLASH);
	m_isAttack = true;
	m_attackFrame = m_attackTime;
	switch (dir)
	{
	case Player::FORWARD:
		m_collider->SetMax(Vector3(0.4f, 0.8f, 1.5f));
		break;
	case Player::BACK:
		m_collider->SetMin(Vector3(-0.4f, 0.0f, -1.5f));
		break;
	case Player::LEFT:
		m_collider->SetMin(Vector3(-1.5f, 0.0f, -0.4f));
		break;
	case Player::RIGHT:
		m_collider->SetMax(Vector3(1.5f, 0.8f, 0.4f));
		break;
	case Player::NON:
		break;
	default:
		break;
	}
}

void Player::CheckGrounded()
{
	std::vector<GameObject*> hit = m_collider->GetHitGameObject();
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i] != nullptr)
		{
			if (hit[i]->tag == "Ground")
			{
				m_isGrounded = true;
			}
		}
	}
	std::vector<GameObject*> exit = m_collider->GetExitGameObject();
	for (int i = 0; i < exit.size(); i++)
	{
		if (exit[i] != nullptr)
		{
			if (exit[i]->tag == "Ground")
			{
				m_isGrounded = false;
			}
		}
	}
}

bool Player::IsAttack()
{
	return m_isAttack;
}