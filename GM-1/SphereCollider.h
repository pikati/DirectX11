#pragma once
#include "Collider.h"
class SphereCollider : public Collider
{
private:
	Vector3 m_center = Vector3::zero;
	float m_radius = 0;
	Transform* m_transform = nullptr;
public:
	SphereCollider();
	~SphereCollider();
	void SystemInitialize()override;
	void Update()override;
	void Draw() override;
	void Finalize() override;
	void SystemFinalize()override;

	void SetCenter(Vector3 center);
	void SetCenter(float x, float y, float z);
	void SetRadius(float radius);
	void SetProperties(Component* c) override;
	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
	void DrawInformation() override;

	Vector3 GetPosition();
	float GetRadius();
};

