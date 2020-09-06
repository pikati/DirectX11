#pragma once
#include "main.h"

class FPS
{
private:
	static const float m_frameRate;
	static DWORD m_dwExecLastTime;
	static DWORD m_dwCurrentTime;
	static DWORD m_dwOldCurrentTime;
	static bool m_isReset;
public:
	static float deltaTime;

	static void Initialize();
	static bool Update();
	static void Finalize();
	static void ResetFPS();
};

