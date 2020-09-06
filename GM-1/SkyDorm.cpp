#include "SkyDorm.h"
#include "main.h"
#include "GameObject.h"
#include "Transform.h"
#include "FPS.h"

void SkyDorm::Update()
{
	m_rad += FPS::deltaTime * 0.05f;
	if (m_rad >= D3DX_PI * 2.0f)
	{
		m_rad -= D3DX_PI * 2.0f;
	}
	gameObject->transform->rotation.y = m_rad * 180.0f / D3DX_PI;
}