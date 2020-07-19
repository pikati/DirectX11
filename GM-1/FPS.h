#pragma once
#include "main.h"

class FPS
{
private:
	static const float m_frameRate;
	static DWORD m_dwExecLastTime;
	static DWORD m_dwCurrentTime;
	static DWORD m_dwOldCurrentTime;
public:
	static void Initialize();
	static bool Update();
	static void Draw();
	static void Finalize();
	static void DrawFPS();
	static float deltaTime;
};

