#include "MainGameManager.h"
#include "manager.h"
#include "Scene.h"
#include "Fade.h"
#include "AudioManager.h"

void MainGameManager::Initialize()
{
	AudioManager::StopSound();
	AudioManager::PlaySound(BGM_MAIN);
	m_fade = CManager::GetScene()->Find("Fade")->GetComponent<Fade>();
	m_fade->FadeOut();
}