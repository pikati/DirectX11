#pragma once
#include "Component.h"
#include "main.h"

class Image : public Component
{
private:
	ID3D11ShaderResourceView* m_texture = nullptr;
	ID3D11Buffer* m_vertexBuffer = nullptr;
	std::string m_textureName = "Asset/Texture/Default/Plane.png";
	float m_width = 100.0f;
	float m_height = 100.0f;
	D3DXCOLOR m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

public:
	void SystemInitialize() override;
	void SystemUpdate() override;
	void Draw() override;
	void Finalize() override;

	void SetTextureName(std::string textureName);
	void SetSize(float width, float height);
	void SetColor(float r, float g, float b, float a);
	void DrawInformation() override;

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

