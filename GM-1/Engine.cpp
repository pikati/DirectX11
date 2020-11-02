#include "Engine.h"
#include <math.h>

#pragma region Vector2

Vector2 Vector2::zero = Vector2(0, 0);
Vector2 Vector2::one = Vector2(1, 1);

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

float Vector2::Length()
{
	return sqrtf(powf(x, 2) + powf(y, 2));
}

float Vector2::LengthSquared()
{
	return powf(x, 2) + powf(y, 2);
}

void Vector2::Normalize()
{
	float length = Length();
	x /= length;
	y /= length;
}

Vector2 Vector2::Normalize(Vector2 vec)
{
	float length = vec.Length();
	vec.x /= length;
	vec.y /= length;
	return vec;
}

float Vector2::Dot(Vector2 vec1, Vector2 vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
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

float Vector3::Length()
{
	return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
}

float Vector3::LengthSquared()
{
	return powf(x, 2) + powf(y, 2) + powf(z, 2);
}

void Vector3::Normalize()
{
	float length = Length();
	x /= length;
	y /= length;
	z /= length;
}

Vector3 Vector3::Normalize(Vector3 vec)
{
	float length = vec.Length();
	vec.x /= length;
	vec.y /= length;
	vec.z /= length;
	return vec;
}

float Vector3::Dot(Vector3 vec1, Vector3 vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 Vector3::Cross(Vector3 vec1, Vector3 vec2)
{
	return Vector3(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
}

const Vector3 Vector3::operator+(const Vector3& vec) const
{
	return Vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}
const Vector3 Vector3::operator-(const Vector3& vec) const
{
	return Vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}
Vector3 operator*(float n, const Vector3 vec)
{
	return Vector3(n * vec.x, n * vec.y, n * vec.z);
}
Vector3 operator*(const Vector3 vec, float n)
{
	return Vector3(n * vec.x, n * vec.y, n * vec.z);
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