#include "SceneManager.h"
#include "LevelLoader.h"
#include "manager.h"
#include "Scene.h"
#include "FPS.h"
#include "ObjectPooler.h"

std::string SceneManager::m_currentSceneName = "";

void SceneManager::LoadScene(std::string sceneName)
{
	m_currentSceneName = sceneName;
	CManager::GetScene()->ChangeScene();
}

void SceneManager::LoadScene()
{
	ObjectPooler::Clear();
	LevelLoader::LoadLevel(CManager::GetScene(), m_currentSceneName.c_str());
	FPS::ResetFPS();
}

std::string SceneManager::CurrentSceneName()
{
	return m_currentSceneName;
}