#pragma once
#include "Component.h"
#include "main.h"
class CPolygon : public Component
{
private:

	ID3D11Buffer*				m_vertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_texture = NULL;

public:
	CPolygon();
	~CPolygon();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
};

