#pragma once
#include "Component.h"
#include "main.h"
#include "renderer.h"
class Fbx;
class Morphing : public Component
{
private:
	VERTEX_3D* m_f1 = nullptr;
	VERTEX_3D* m_f2 = nullptr;
	int m_vCount = 0;
	int m_iCount = 0;
	ID3D11ShaderResourceView* m_tex1 = nullptr;
	ID3D11ShaderResourceView* m_tex2 = nullptr;
	float m_raito = 0.0f;
	bool m_isUp = true;
	ID3D11Buffer* pVB = nullptr;
	ID3D11Buffer* pIB = nullptr;
	float m_morphSpeed = 0.2f;

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	void DrawInformation() override;

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

