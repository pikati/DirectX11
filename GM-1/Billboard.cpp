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

	gameObject->transform->position = Vector3(0.0f, 0.0f, 0.0f);
	gameObject->transform->rotation = Vector3(0.0f, 0.0f, 0.0f);
	gameObject->transform->scale = Vector3(1.0f, 1.0f, 1.0f);
}

void Billboard::Update()
{
}

void Billboard::Draw()
{
	/*Camera* camera = CManager::GetScene()->Find<Camera>()->GetComponent<Camera>();

	D3DXMATRIX view = camera->GetViewMatrix();
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;*/

	Vector3 cameraPosition = CManager::GetScene()->Find<Camera>()->transform->position;
	float diffX = cameraPosition.x - gameObject->transform->position.x;
	float diffY = cameraPosition.y - gameObject->transform->position.y;
	float diffZ = cameraPosition.z - gameObject->transform->position.z;
	float rotationX = atan2f(diffY, diffZ);
	float rotationY = atan2f(diffZ, diffX);
	if (rotationX < 0)
	{
		rotationX += 2 * 3.14f;
	}
	if (rotationY < 0)
	{
		rotationY += 2 * 3.14f;
	}
	gameObject->transform->rotation.x = rotationX * 3.14f / 180.0f;
	gameObject->transform->rotation.y = rotationY * 3.14f / 180.0f;
}

void Billboard::Finalize()
{
}