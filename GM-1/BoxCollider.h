#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
private:
	Vector3 m_max = Vector3::one;
	Vector3 m_min = Vector3(-1, -1, -1);
	Vector3 m_center = Vector3::zero;
	Transform* m_transform = nullptr;

public:
	BoxCollider();
	~BoxCollider();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	Vector3 GetPosition() override;
	void SetMax(Vector3 max);
	void SetMin(Vector3 min);

	float GetLength(int i);
	Vector3 GetAxis(int i);

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

