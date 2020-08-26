#pragma once
#include "Component.h"
#include <Vector>
class Collider : public Component
{
private:
	static std::vector<Collider*> m_colliders;
	bool m_isCollision = false;
	bool m_isDestroy = false;

	static void RunCollisionDetection(Collider* c1, Collider* c2);
	static void Sphere2Sphere(Collider* c1, Collider* c2);
	static void Sphere2Mesh(Collider* c1, Collider* c2);
	static void Sphere2AABB(Collider* c1, Collider* c2);
	static void AABB2AABB(Collider* c1, Collider* c2);
	
protected:
	enum ColliderType
	{
		Sphere,
		Mesh,
		Aabb,
		Max
	};
	bool m_isCollisionThisFrame = false;
	int m_colliderID = -1;
	GameObject* m_hitObject = nullptr;
	GameObject* m_exitObject = nullptr;
	ColliderType m_colliderType;

	int SetCollider(Collider* collider);
	void DeleteCollider(int colliderID);

public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
	virtual Vector3 GetPosition() = 0;
	virtual float GetRadius();
	virtual void LoadProperties(const rapidjson::Value& inProp);
	virtual void SavePropaties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp);
	GameObject* GetHitGameObject();
	GameObject* GetExittGameObject();
	bool IsCollision();
	static void UpdateCollision();
	void Destroy();
};

