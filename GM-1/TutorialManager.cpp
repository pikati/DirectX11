#include "TutorialManager.h"
#include "main.h"
#include "SceneManager.h"
#include "input.h"
#include "manager.h"
#include "Scene.h"
#include "Fade.h"
#include "AudioManager.h"

void TutorialManager::Initialize()
{
	m_fade = CManager::GetScene()->Find("Fade")->GetComponent<Fade>();
	m_fade->FadeOut();
}

void TutorialManager::Update()
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

void TutorialManager::Draw()
{

}

void TutorialManager::Finalize()
{

}