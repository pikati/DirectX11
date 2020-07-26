#pragma once
#include "Component.h"
#include "main.h"
class Texture : public Component
{
private:
	static int m_maxID;
	std::string m_fileName;
	ID3D11Buffer* m_vertexBuffer = NULL;
	ID3D11ShaderResourceView* m_texture = NULL;
	int m_id = -1;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void SetFileName(std::string fileName);
	void SetID(int id);
	int GetTextureID();
	ID3D11ShaderResourceView* GetTexture();

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

