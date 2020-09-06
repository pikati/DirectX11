#include "TitleText.h"
#include "FPS.h"
#include "main.h"
#include "GameObject.h"
#include "Transform.h"

void TitleText::Update()
{
	m_num += FPS::deltaTime * 5.0f;
	if (m_num > 2.0f * D3DX_PI)
	{
		m_num -= 2.0f * D3DX_PI;
	}
	float y = sinf(m_num);
	gameObject->transform->position.y += y;
}