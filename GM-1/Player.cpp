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
	gameObject->transform->scale    = Vector3(1.0f, 1.0f, 1.0f);
	ObjectPooler::LoadGameObject("Asset/Prefabs/Bullet.prefab");

	//quaternion‚Ì‰Šú‰»
	//D3DXQuaternionIdentity(&m_quaternion);

	m_collider = gameObject->GetComponent<AABB>();
	
	m_animation = gameObject->GetComponent<Animation>();
	m_animation->SetSpeed(WALK, 2);
}

void Player::Update()
{
	Vector3 forward = gameObject->GetForward();
	

	Move();
	
	
	if (CInput::GetKeyTrigger('P'))
	{
		GameObject* obj;
		obj = ObjectPooler::CreatePrefab("Bullet");
		obj->transform->position = gameObject->transform->position;
		m_animation->SetState(SHOOT);
	}
	if (m_collider->IsCollision())
	{
		if (m_collider->GetHitGameObject()->tag == "Ground")
		{
			m_isGrounded = true;
		}
	}
	else
	{
		m_isGrounded = false;
	}

	//D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
}

void Player::Draw()
{
}

void Player::Finalize()
{
}

void Player::Move()
{
	m_velocity = Vector3::zero;
	if (CInput::GetKeyPress('A'))
	{
		//gameObject->transform->position += Vector3(-0.1f, 0.0f, 0.0f);
		//gameObject->transform->rotation.y += -180.0f * FPS::deltaTime;
		/*D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(0, 1.0f, 0), 0.1f);
		m_quaternion *= quat;*/
		m_velocity.x += -SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 270.0f;
	}
	if (CInput::GetKeyPress('D'))
	{
		//gameObject->transform->position += Vector3(0.1f, 0.0f, 0.0f);
		//gameObject->transform->rotation.y += 180.0f * FPS::deltaTime;
		/*D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(0, 1.0f, 0), -0.1f);
		m_quaternion *= quat;*/
		m_velocity.x += SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 90.0f;
	}
	if (CInput::GetKeyPress('W'))
	{
		//gameObject->transform->position += Vector3(0.0f, 0.0f, 0.1f);
		//gameObject->transform->position += forward * 10.0f * FPS::deltaTime * -1;
		//m_velocity += forward * 10.0f * FPS::deltaTime * -1;
		m_velocity.z += SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 0.0f;
	}
	if (CInput::GetKeyPress('S'))
	{
		//gameObject->transform->position += Vector3(0.0f, 0.0f, -0.1f);
		//gameObject->transform->position += forward * 10.0f * FPS::deltaTime;
		//m_velocity += forward * 10.0f * FPS::deltaTime;
		m_velocity.z += -SPEED * FPS::deltaTime;
		gameObject->transform->rotation.y = 180.0f;
	}
	m_velocity.y += -m_gravity * FPS::deltaTime;
	if (m_collider->IsCollision())
	{
		if (m_collider->GetHitGameObject()->tag == "Ground")
		{
			m_velocity.y = 0;
		}
	}
	gameObject->transform->position += m_velocity;

	if (m_velocity.x != 0 || m_velocity.z != 0)
	{
		m_animation->SetState(WALK);
	}
	else
	{
		m_animation->SetState(IDOL);
	}
}