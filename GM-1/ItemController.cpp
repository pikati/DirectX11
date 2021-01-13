#include "ItemController.h"
#include "manager.h"
#include "Scene.h"
#include "Item.h"
#include "SceneManager.h"
#include "AABB.h"
#include "Fade.h"

void ItemController::Initialize()
{
	m_collider = gameObject->GetComponent<AABB>();
	//m_fade = CManager::GetScene()->Find("Fade")->GetComponent<Fade>();
}

void ItemController::Update()
{
	if (m_isGetApple && m_isGetStrawberry && m_isGetBanana && !m_isChangeScene)
	{
		//m_fade->FadeIn();
		m_isChangeScene = true;
	}
	if (m_isChangeScene)
	{
		SceneManager::LoadScene("Asset/Scene/result.scene");
	}
}

void ItemController::GetApple()
{
	m_isGetApple = true;
}

void ItemController::GetStrawberry()
{
	m_isGetStrawberry = true;
}

void ItemController::GetBanana()
{
	m_isGetBanana = true;
}