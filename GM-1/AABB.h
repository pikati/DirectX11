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
	ID3D11Buffer* m_vb1;
	ID3D11Buffer* m_vb2;
	LINE_VERTEX m_v[14];
	Texture* m_texture;
	bool m_isKinematic = false;
public:
	AABB();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void SetMax(Vector3 max);
	void SetMin(Vector3 min);
	void SetMax(float x, float y, float z);
	void SetMin(float x, float y, float z);
	void SetPosition(Vector3 pos);
	Vector3 GetMax();
	Vector3 GetMin();
	Vector3 GetPosition();
	void SetKinematic(bool on);
	bool IsKinematic();

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

