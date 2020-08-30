#include "TitleManager.h"
#include "main.h"
#include "SceneManager.h"
#include "input.h"
#include "manager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Fade.h"
#include "AudioManager.h"

void TitleManager::Initialize()
{
	AudioManager::PlaySound(BGM_TITLE);
	m_fade = CManager::GetScene()->Find("Fade")->GetComponent<Fade>();
	m_fade->FadeOut();
}

void TitleManager::Update()
{
	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		m_fade->FadeIn();
		m_isChangeScene = true;
		AudioManager::PlaySound(SE_DECIDE);
	}
	if (m_isChangeScene && !m_fade->IsFading())
	{
		SceneManager::LoadScene("Asset/Scene/stage1.scene");
	}
}

void TitleManager::Draw()
{

}

void TitleManager::Finalize()
{

}