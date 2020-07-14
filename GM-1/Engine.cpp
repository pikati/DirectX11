#include "Engine.h"

#pragma region Vector2
Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Vector2::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

const Vector2 Vector2::operator+(const Vector2& vec) const
{
	return Vector2(x + vec.x, y + vec.y);
}

const Vector2 Vector2::operator-(const Vector2& vec) const
{
	return Vector2(x - vec.x, y - vec.y);
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}
#pragma endregion

#pragma region Vector3
Vector3 Vector3::back = Vector3(0.0f, 0.0f, 1.0f);
Vector3 Vector3::down = Vector3(0.0f, -1.0f, 0.0f);
Vector3 Vector3::forward = Vector3(0.0f, 0.0f, 1.0f);
Vector3 Vector3::left = Vector3(-1.0f, 0.0f, 0.0f);
Vector3 Vector3::one = Vector3(1.0f, 1.0f, 1.0f);
Vector3 Vector3::right = Vector3(1.0f, 0.0f, 0.0f);
Vector3 Vector3::up = Vector3(0.0f, 1.0f, 0.0f);
Vector3 Vector3::zero = Vector3(0.0f, 0.0f, 0.0f);

Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
void Vector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
const Vector3 Vector3::operator+(const Vector3& vec) const
{
	return Vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}
const Vector3 Vector3::operator-(const Vector3& vec) const
{
	return Vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}
//const Vector3 Vector3::operator*(const float n) const
//{
//	return Vector3(this->x * n, this->y * n, this->z * n);
//}
//
//const Vector3 Vector3::operator/(const float n) const
//{
//	return Vector3(this->x / n, this->y / n, this->z / n);
//}
Vector3& Vector3::operator+=(const Vector3& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

#pragma endregion

#pragma region Vector4
Vector4::Vector4()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}
Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
void Vector4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

const Vector4 Vector4::operator+(const Vector4& vec) const
{
	return Vector4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);
}
const Vector4 Vector4::operator-(const Vector4& vec) const
{
	return Vector4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);
}
Vector4& Vector4::operator+=(const Vector4& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}
#pragma endregion