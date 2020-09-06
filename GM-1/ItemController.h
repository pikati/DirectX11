#pragma once
#include "Component.h"
class AABB;
class Fade;

class ItemController :	public Component
{
private:
	bool m_isGetApple = false;
	bool m_isGetStrawberry = false;
	bool m_isGetBanana = false;
	bool m_isChangeScene = false;
	AABB* m_collider = nullptr;
	Fade* m_fade;
public:
	void Initialize();
	void Update();

	void GetApple();
	void GetStrawberry();
	void GetBanana();
};

