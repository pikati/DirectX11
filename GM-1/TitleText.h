#pragma once
#include "Component.h"
class TitleText : public Component
{
private:
	float m_num = 0;
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
};

