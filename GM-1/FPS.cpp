#include "FPS.h"


const float FPS::m_frameRate = 60;
DWORD FPS::m_dwCurrentTime = 0;
DWORD FPS::m_dwExecLastTime = 0;
DWORD FPS::m_dwOldCurrentTime = 0;
float FPS::deltaTime = 0;

void FPS::Initialize()
{
	timeBeginPeriod(1);
	m_dwExecLastTime = timeGetTime();
}

bool FPS::Update()
{
	m_dwOldCurrentTime = m_dwCurrentTime;
	m_dwCurrentTime = timeGetTime();
	if (m_dwCurrentTime - m_dwExecLastTime >= (1000 / m_frameRate))
	{
		deltaTime = (m_dwCurrentTime - m_dwExecLastTime) / 1000.0f;
		m_dwExecLastTime = m_dwCurrentTime;
		return true;
	}
	else return false;
}

void FPS::Draw()
{
}

void FPS::Finalize()
{
	timeEndPeriod(1);
}