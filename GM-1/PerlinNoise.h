/**
 *	@file		PerlinNoise.h
 *	@brief		PerlinNoiseクラスのヘッダ コピペ
 *	@authotr	https://qiita.com/keny30827/items/f4e29a4a90779cf94da6
*/

#pragma once

//指定範囲でクリップする.
#define CLIP(e,l,h) (min(max(e,l),h))

//配列の要素数取得.
#define COUNTOF(a) ( sizeof( a ) / sizeof( a[0] ) )

#define HASH_CODE_MAX       (256)
#define HASH_CODE_TABLE_NUM     (HASH_CODE_MAX*2)

//ベクトル構造体.
#define VECTOR SVector2D<float>
template <class T>
struct SVector2D
{
	typedef T DataType;
	T x;
	T y;
	SVector2D() { Init(); }
	void Init()
	{
		x = T();
		y = T();
	}
	SVector2D   operator +  (const SVector2D& e) const { SVector2D tmp; tmp.x = x + e.x; tmp.y = y + e.y; return tmp; }
	SVector2D&  operator += (const SVector2D& e) { x += e.x; y += e.y; return (*this); }
	SVector2D   operator -  (const SVector2D& e) const { SVector2D tmp; tmp.x = x - e.x; tmp.y = y - e.y; return tmp; }
	SVector2D&  operator -= (const SVector2D& e) { x -= e.x; y -= e.y; return (*this); }
	T           operator *  (const SVector2D& e) const { return (x * e.x) + (y * e.y); }
	SVector2D&  operator *= (const int e) { x *= e; y *= e; return (*this); }
	SVector2D&  operator *= (const float e) { x *= e; y *= e; return (*this); }
	SVector2D&  operator /= (const int e) { x /= e; y /= e; return (*this); }
	SVector2D&  operator /= (const float e) { x /= e; y /= e; return (*this); }
};

class PerlinNoise
{
private:
	int m_HashCode[HASH_CODE_TABLE_NUM] = {};

	float Fade(float t);
	float Lerp(float a, float b, float t);
	float Grad(unsigned int hash, float a, float b);
	unsigned int GetHash(int x, int y);
	float GetPerlinNoise(float x, float y);
public:
	void SettingHash(unsigned int seed);
	float OctavePerlinNoise(float x, float y);
	PerlinNoise();
	~PerlinNoise();
};

