#pragma once
#include "Collider.h"
#include "main.h"
#include "renderer.h"

class MeshCollider : public Collider
{
private:
	Vector3* m_mesh;
	int m_sizeX;
	int m_sizeZ;
public:
	MeshCollider();
	~MeshCollider();

	void SystemInitialize() override;
	void Update() override;
	void SystemFinalize() override;

	void SetVertices(VERTEX_3D* vertcies, int sizeX, int sizeZ);
	Vector3* GetVertices();
	int GetSizeX();
	int GetSizeZ();
	Vector3 GetPosition() override;
};

