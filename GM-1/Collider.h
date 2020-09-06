#pragma once
#include "Component.h"
#include <Vector>
class Collider : public Component
{
private:
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
	int m_colliderID = -1;
	bool m_isCollisionThisFrame = false;

	std::vector<GameObject*> m_hitObject;
	std::vector<GameObject*> m_exitObject;
	ColliderType m_colliderType;

	int SetCollider(Collider* collider);
	void DeleteCollider(int colliderID);

public:
	static std::vector<Collider*> m_colliders;


	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();
	virtual Vector3 GetPosition() = 0;
	virtual float GetRadius();
	virtual void LoadProperties(const rapidjson::Value& inProp);
	virtual void SavePropaties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp);
	std::vector<GameObject*> GetHitGameObject();
	std::vector<GameObject*> GetExitGameObject();
	bool IsCollision();
	static void UpdateCollision();
	static void ResetExitCollision();
	void Destroy();
};

