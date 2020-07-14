#pragma once
#include "Component.h"
#include "main.h"

class Field : public Component
{
private:
	ID3D11Buffer* m_vertexBuffer = NULL;
	ID3D11ShaderResourceView* m_texture = NULL;
public:
	Field();
	~Field();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
};

