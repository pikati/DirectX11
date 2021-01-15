#include "MeshCollider.h"

MeshCollider::MeshCollider()
{
	m_colliderType = Mesh;
	m_mesh = nullptr;
}

MeshCollider::~MeshCollider()
{

}

void MeshCollider::SystemInitialize()
{
	m_colliderID = SetCollider(this);
}

void MeshCollider::Update()
{
	m_isCollisionThisFrame = false;
	Collider::Update();
}

void MeshCollider::SystemFinalize()
{
	DeleteCollider(m_colliderID);
	if (!m_mesh)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}
}

void MeshCollider::SetVertices(VERTEX_3D* vertcies, int sizeX, int sizeZ)
{
	m_sizeX = sizeX;
	m_sizeZ = sizeZ;
	m_mesh = new Vector3[(m_sizeX + 1) * (m_sizeZ + 1)];
	for (int i = 0; i < (m_sizeX + 1) * (m_sizeZ + 1); i++)
	{
		m_mesh[i] = Vector3(vertcies[i].Position.x, vertcies[i].Position.y, vertcies[i].Position.z);
	}
}

Vector3* MeshCollider::GetVertices()
{
	return m_mesh;
}

int MeshCollider::GetSizeX()
{
	return m_sizeX;
}

int MeshCollider::GetSizeZ()
{
	return m_sizeZ;
}

Vector3 MeshCollider::GetPosition()
{
	return m_mesh[(m_sizeX + 1) * (m_sizeZ + 1) / 2];
}