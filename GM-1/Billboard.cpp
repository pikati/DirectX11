#include "Billboard.h"
#include "main.h"
#include "renderer.h"
#include "Transform.h"
#include "Camera.h"
#include "manager.h"
#include "GameObject.h"
#include "Scene.h"
#include <math.h>

void Billboard::Initialize()
{
	gameObject->transform->rotation = Vector3(-90.0f, 0.0f, 0.0f);
}

void Billboard::Update()
{
	Vector3 cameraPosition = CManager::GetScene()->Find<Camera>()->transform->position;
	float diffX = -cameraPosition.x + gameObject->transform->position.x;
	float diffZ = -cameraPosition.z + gameObject->transform->position.z;
	float rotationY = atan2f(diffZ, diffX);
	
	if (rotationY < 0)
	{
		rotationY += D3DX_PI * 2.0f;
	}
	else if (rotationY > D3DX_PI * 2.0f)
	{
		rotationY -= D3DX_PI * 2.0f;
	}
	gameObject->transform->rotation.y = -(rotationY * 180.0f / D3DX_PI - 90.0f);
}