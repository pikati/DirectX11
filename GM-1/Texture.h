#pragma once
#include "Component.h"
#include "main.h"

class Texture : public Component
{
private:
	static int m_maxID;
	std::string m_fileName;
	ID3D11ShaderResourceView* m_texture = NULL;
	void LoadTexture(std::string fileName);
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void SetID(int id);
	int GetTextureID();
	void SetTextureName(std::string textureName);
	ID3D11ShaderResourceView* GetTexture();

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

