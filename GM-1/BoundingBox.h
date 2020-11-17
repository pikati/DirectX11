#pragma once
#include "Engine.h"
class Transform;
class BoundingBox
{
public:
	Vector3 m_max = { 0, 0, 0 };
	Vector3 m_min = { 0, 0, 0 };
	void Draw(Transform* t);
};

