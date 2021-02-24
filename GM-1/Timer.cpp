#include "Timer.h"

Timer::Timer(float interval)
	:m_intervalTime(interval)
{
}

bool Timer::Update(float timeScale = 1.0f)
{
	m_elaspedTime += FPS::deltaTime * timeScale;
	return IsTimeUp();
}
