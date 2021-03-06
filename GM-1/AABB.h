#pragma once
#include "Collider.h"
#include "main.h"
class Texture;
struct LINE_VERTEX
{
	Vector3 pos;
	Vector4 color;
};
class AABB : public Collider
{
private:
	Vector3 m_max;
	Vector3 m_min;
	Vector3 m_pos;
	ID3D11Buffer* m_vb1 = nullptr;
	ID3D11Buffer* m_vb2 = nullptr;
	LINE_VERTEX m_v[14];
	Texture* m_texture;
public:
	AABB();
	~AABB();
	void SystemInitialize() override;
	void Update() override;
	void Draw() override;
	void SystemFinalize() override;
	void SetMax(Vector3 max);
	void SetMin(Vector3 min);
	void SetMax(float x, float y, float z);
	void SetMin(float x, float y, float z);
	void SetPosition(Vector3 pos);
	Vector3 GetMax();
	Vector3 GetMin();
	Vector3 GetPosition();
	float GetHeight();
	void DrawInformation() override;

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

