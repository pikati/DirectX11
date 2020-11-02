#pragma once
#include "Component.h"
#include "main.h"
#include "renderer.h"

class MeshField : public Component
{
private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;
	VERTEX_3D* m_vertices;
	std::string m_fileName;
	int m_x = 1;
	int m_z = 1;

	void CalcNormals();
	void SetNormal(int index, Vector3 v1, Vector3 v2);
public:
	void Initialize() override;
	void Draw() override;
	void Finalize() override;

	void SetTexture(std::string fileName);
	void SetX(int x);
	void SetZ(int z);
	void SetXZ(int x, int z);
	VERTEX_3D* GetVertices();
};

