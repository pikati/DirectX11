#pragma once
#include "FPS.h"
class Timer
{
private:
	float m_intervalTime = 0.0f;
	float m_elaspedTime = 0.0f;
public:
	Timer() = delete;
	Timer(float interval);

	bool Update(float timeScale);
	void Reset();
	void Reset(float interval);
	bool IsTimeUp();
};

