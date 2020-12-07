#pragma once
#include <string>
class Hierarchy
{
private:
	static bool m_isSaveMenu;
	static bool m_isLoadMenu;
	static int* m_layerMax;

	static std::string m_savePath;
	static std::string m_loadPath;

	static void DispLoadMenu();
	static void DispSaveMenu();
public:
	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();
};

