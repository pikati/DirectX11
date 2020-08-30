#pragma once
#include "Component.h"
class SkyDorm : public Component
{
private:
	float m_rad = 0;
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
};

