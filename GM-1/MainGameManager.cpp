#include "MainGameManager.h"
#include "manager.h"
#include "Scene.h"
#include "Fade.h"
#include "AudioManager.h"

void MainGameManager::Initialize()
{
	AudioManager::StopSound();
	AudioManager::PlaySound(BGM_MAIN);
	GameObject* obj = CManager::GetScene()->Find("Fade");
	m_fade = obj->GetComponent<Fade>();
	m_fade->FadeOut();
}