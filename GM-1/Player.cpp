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
#include "Scene.h"
#include "CameraController.h"
#include "Item.h"
#include "ItemController.h"
#include "AudioManager.h"

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
	m_camera = nullptr;
	m_itemController = nullptr;
	m_velocity = Vector3::zero;
}

Player::~Player()
{

}


void Player::Initialize()
{
	m_collider = gameObject->GetComponent<AABB>();
	m_animation = gameObject->GetComponent<Animation>();
	m_itemController = gameObject->GetComponent<ItemController>();
	m_camera = CManager::GetScene()->Find("MainCamera")->GetComponent<CameraController>();

	m_animation->SetSpeed(WALK, 2);
}

void Player::Update()
{
	Vector3 forward = gameObject->GetForward();

	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		Shot();
	}

	if (CInput::GetKeyTrigger('K'))
	{
		Jump();
	}
	if (CInput::GetKeyTrigger('C'))
	{
		m_animation->SetState(SLASH);
	}
	if (CInput::GetKeyTrigger('V'))
	{
		m_animation->SetState(STAB);
	}
	if (CInput::GetKeyTrigger('B'))
	{
		m_animation->SetState(DAMAGE);
	}
	Move();

	if (m_isGrounded)
	{
		m_velocity.y = 0;
	}
	gameObject->transform->position += m_velocity;
	if (gameObject->transform->position.y < -20)
	{
		gameObject->transform->position = m_startPosition;
		m_velocity.y = 0;
	}
}

void Player::Move()
{
	float rotation = m_camera->GetRotation();
	m_velocity.x = 0;
	m_velocity.z = 0;
	
	if (CInput::GetKeyPress('A'))
	{
		if (CInput::GetKeyPress('W'))
		{
			m_velocity.x += sinf(-D3DX_PI * 0.75f - rotation) * SPEED * FPS::deltaTime;
			m_velocity.z -= cosf(-D3DX_PI * 0.75f - rotation) * SPEED * FPS::deltaTime;
			m_direction.y = rotation - D3DX_PI * 0.25f;
		}
		else if (CInput::GetKeyPress('S'))
		{
			m_velocity.x += sinf(-D3DX_PI * 0.25f - rotation) * SPEED * FPS::deltaTime;
			m_velocity.z -= cosf(-D3DX_PI * 0.25f - rotation) * SPEED * FPS::deltaTime;
			m_direction.y = rotation - D3DX_PI * 0.75f;
		}
		else
		{
			m_velocity.x += sinf(-D3DX_PI * 0.5f - rotation) * SPEED * FPS::deltaTime;
			m_velocity.z -= cosf(-D3DX_PI * 0.5f - rotation) * SPEED * FPS::deltaTime;
			m_direction.y = rotation + D3DX_PI * -0.5f;
		}
	}
	else if (CInput::GetKeyPress('D'))
	{
		if (CInput::GetKeyPress('W'))
		{
			m_velocity.x += sinf(D3DX_PI * 0.75f - rotation) * SPEED * FPS::deltaTime;
			m_velocity.z -= cosf(D3DX_PI * 0.75f - rotation) * SPEED * FPS::deltaTime;
			m_direction.y = rotation + D3DX_PI * 0.25f;
		}
		else if (CInput::GetKeyPress('S'))
		{
			m_velocity.x += sinf(D3DX_PI * 0.25f - rotation) * SPEED * FPS::deltaTime;
			m_velocity.z -= cosf(D3DX_PI * 0.25f - rotation) * SPEED * FPS::deltaTime;
			m_direction.y = rotation + D3DX_PI * 0.75f;
		}
		else
		{
			m_velocity.x += sinf(D3DX_PI * 0.5f - rotation) * SPEED * FPS::deltaTime;
			m_velocity.z -= cosf(D3DX_PI * 0.5f - rotation) * SPEED * FPS::deltaTime;
			m_direction.y = rotation - D3DX_PI * -0.5f;
		}
	}
	else if (CInput::GetKeyPress('W'))
	{
		m_velocity.x += sinf(D3DX_PI - rotation) * SPEED * FPS::deltaTime;
		m_velocity.z -= cosf(D3DX_PI - rotation) * SPEED * FPS::deltaTime;
		m_direction.y = rotation;
	}
	else if (CInput::GetKeyPress('S'))
	{
		m_velocity.x += sinf(-rotation) * SPEED * FPS::deltaTime;
		m_velocity.z -= cosf(-rotation) * SPEED * FPS::deltaTime;
		m_direction.y = rotation + D3DX_PI * 1.0f;
	}

	float diffRotationY = m_direction.y * (180.0f / D3DX_PI) - gameObject->transform->rotation.y;
	if (diffRotationY > 360.0f)
	{
		diffRotationY -= 360.0f;
	}
	else if (diffRotationY < 0)
	{
		diffRotationY += 360.0f;
	}
	gameObject->transform->rotation.y += diffRotationY;

	//m_velocity.y += -m_gravity * FPS::deltaTime * 0.125f;
	//gameObject->transform->position += m_velocity;

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
	obj->transform->position.y += 0.5f;
	m_animation->SetState(SHOOT);
}
 
void Player::Jump()
{
	if (m_isGrounded)
	{
		AudioManager::PlaySound(SE_JUMP);
		m_velocity.y += 0.25f;
		m_isGrounded = false;
	}
}

void Player::OnCollisionEnter(GameObject* obj)
{
	if (obj->tag == "Ground")
	{
		m_isGrounded = true;
	}
	else if (obj->tag == "Item")
	{
		AudioManager::PlaySound(SE_GET);
		switch (obj->GetComponent<Item>()->GetItemType())
		{
		case ItemType::Apple:
			m_itemController->GetApple();
			break;
		case ItemType::Strawberry:
			m_itemController->GetStrawberry();
			break;
		case ItemType::Banana:
			m_itemController->GetBanana();
			break;
		default:
			break;
		}
	}
}

void Player::OnCollisionExit(GameObject* obj)
{
	if (obj->tag == "Ground")
	{
		m_isGrounded = false;
	}
}