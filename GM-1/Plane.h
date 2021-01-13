#pragma once
#include "Component.h"
#include "main.h"
#include "renderer.h"

class Texture;

class Plane : public Component
{
private:
	Vector3 m_meshNormal;
	Vector3 m_center;
	ID3D11Buffer* m_vertexBuffer = nullptr;
	std::string m_textureName = "Asset/Texture/Default/Plane.png";
	ID3D11ShaderResourceView* m_tex;
	SHADER_TYPE m_shaderType = SHADER_TYPE::Default;
	void CalculateNormal(Vector3 a, Vector3 b, Vector3 c);
	void SetCenter(Vector3 a, Vector3 b, Vector3 c, Vector3 d);
	void DrawNormal();
public:
	void SystemInitialize();
	void Draw();
	void Finalize();
	void SetTextureName(std::string name);
	void DrawInformation() override;

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

