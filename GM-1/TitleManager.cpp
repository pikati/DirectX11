#include "TitleManager.h"
#include "main.h"
#include "SceneManager.h"
#include "input.h"
#include "manager.h"
#include "Scene.h"
#include "Fade.h"
#include "AudioManager.h"

void TitleManager::Initialize()
{
	AudioManager::StopSound();
	AudioManager::PlaySound(BGM_TITLE);
	/*m_fade = CManager::GetScene()->Find("Fade")->GetComponent<Fade>();
	m_fade->FadeOut();*/
}

void TitleManager::Update()
{
	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		//m_fade->FadeIn();
		m_isChangeScene = true;
		AudioManager::PlaySound(SE_DECIDE);
		SceneManager::LoadScene("Asset/Scene/MainGame.scene");
	}
	/*if (m_isChangeScene && !m_fade->IsFading())
	{*/
	//}
}