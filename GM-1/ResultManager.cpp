#include "ResultManager.h"
#include "main.h"
#include "SceneManager.h"
#include "input.h"
#include "manager.h"
#include "Scene.h"
#include "Fade.h"
#include "AudioManager.h"

void ResultManager::Initialize()
{
	AudioManager::StopSound();
	AudioManager::PlaySound(BGM_RESULT);
	/*m_fade = CManager::GetScene()->Find("Fade")->GetComponent<Fade>();
	m_fade->FadeOut();*/
}

void ResultManager::Update()
{
	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		//m_fade->FadeIn();
		m_isChangeScene = true;
		AudioManager::PlaySound(SE_DECIDE);
		SceneManager::LoadScene("Asset/Scene/title.scene");
	}
	/*if (m_isChangeScene && !m_fade->IsFading())
	{
	}*/
}