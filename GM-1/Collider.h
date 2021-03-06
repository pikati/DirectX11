#pragma once
#include "Component.h"
#include <Vector>
class Collider : public Component
{
private:
	bool m_isCollision = false;
	GameObject* m_collisionObject = nullptr;
	bool m_isDestroy = false;

	static void RunCollisionDetection(Collider* c1, Collider* c2);
	static void Sphere2Sphere(Collider* c1, Collider* c2);
	static void Sphere2Mesh(Collider* c1, Collider* c2);
	static void Sphere2AABB(Collider* c1, Collider* c2);
	static void AABB2AABB(Collider* c1, Collider* c2);
	static void Box2Mesh(Collider* c1, Collider* c2);
	static void Box2Sphere(Collider* c1, Collider* c2);
	static void Box2Box(Collider* c1, Collider* c2);
	static bool CollisionLineSegumentAndPlane(Vector3 a, Vector3 b, Vector3 v0, Vector3 v1, Vector3 v2);
	static Vector3 CalcIntersectionLineSegmentAndPlane(Vector3 a, Vector3 b, Vector3 v0, Vector3 v1, Vector3 v2);
	static float CalcDistancePointAndPlane(Vector3 p, Vector3 v0, Vector3 v1, Vector3 v2);
	static bool DetectPointIsEnclosedByPolygon(Vector3 p, Vector3 v0, Vector3 v1, Vector3 v2);
	static float LenSegOnSeparateAxis(Vector3 sep, Vector3 e1, Vector3 e2, Vector3 e3);
	static bool CollisionOBBSparationAxis(Vector3 separationAxis, Vector3 axsisLength, Vector3 l1, Vector3 l2, Vector3 l3, Vector3 intarval);
	static bool CollisionOBBSparationAxis(Vector3 separationAxisA, Vector3 SeparationAxisB, Vector3 l1, Vector3 l2, Vector3 l3, Vector3 l4, Vector3 interval);
protected:
	enum ColliderType
	{
		Sphere,
		Mesh,
		Aabb,
		Box,
		Max
	};
	int m_colliderID = -1;
	bool m_isCollisionThisFrame = false;
	ColliderType m_colliderType;
	bool m_isDraw = false;

	int SetCollider(Collider* collider);
	void DeleteCollider(int colliderID);

public:
	static std::vector<Collider*> m_colliders;


	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();
	static void FinalizeCollider();
	virtual Vector3 GetPosition() = 0;
	virtual float GetRadius();
	virtual void LoadProperties(const rapidjson::Value& inProp);
	virtual void SavePropaties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp);
	bool IsCollision();
	static void UpdateCollision();
	void Destroy();
};

