#pragma once
#include "Component.h"
#include "main.h"
class SpriteEffect : public Component
{
private:
	ID3D11Buffer* m_vertexBuffer = NULL;
	ID3D11ShaderResourceView* m_texture = NULL;
	int m_count = 0;
	int m_maxCount = 0;
public:
	SpriteEffect();
	~SpriteEffect();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void SetMaxCount(int count);
};

