#pragma once
#include "Component.h"
class Fade;
class ResultManager : public Component
{
private:
	Fade* m_fade = nullptr;
	bool m_isChangeScene = false;
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
};

