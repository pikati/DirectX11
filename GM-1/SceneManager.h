#pragma once
#include <string>

class SceneManager
{
private:
	static std::string m_currentSceneName;
public:
	static void LoadScene(std::string sceneName);
	static void LoadScene();
	static std::string CurrentSceneName();
};

