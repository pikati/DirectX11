/**
 *	@file		PerlinNoise.cpp
 *	@brief		PerlinNoiseクラスの実装 コピペ
 *	@authotr	https://qiita.com/keny30827/items/f4e29a4a90779cf94da6
*/

#include "PerlinNoise.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

PerlinNoise::PerlinNoise()
{
}


PerlinNoise::~PerlinNoise()
{
}

unsigned int PerlinNoise::GetHash(int x, int y)
{
	x %= HASH_CODE_MAX;
	y %= HASH_CODE_MAX;
	return m_HashCode[x + m_HashCode[y]];
}

void PerlinNoise::SettingHash(unsigned int seed)
{
	//乱数ライブラリ初期化.
	srand(seed);

	//ハッシュコード初期化.
	memset(m_HashCode, 0, sizeof(unsigned int) * COUNTOF(m_HashCode));

	//ランダムテーブル生成.
	const int TABLE_NUM = HASH_CODE_MAX;
	unsigned int randomTable[TABLE_NUM] = {};
	for (int i = 0; i < COUNTOF(randomTable); ++i) {
		randomTable[i] = rand() % HASH_CODE_MAX;
	}

	//ハッシュコード生成.
	for (int i = 0; i < COUNTOF(m_HashCode); ++i) {
		m_HashCode[i] = randomTable[i % TABLE_NUM];
	}
}

float PerlinNoise::Fade(float t)
{
	//Ken Perlin氏(パーリンノイズを作った人)が考えだした補間関数.
	//6x^5 - 15x^4 + 10x^3.
	return (6 * powf(t, 5) - 15 * powf(t, 4) + 10 * powf(t, 3));
}

float PerlinNoise::Lerp(float a, float b, float t)
{
	return (a + (b - a) * t);
}

float PerlinNoise::Grad(unsigned int hash, float a, float b)
{
	unsigned int key = hash % 0x4;
	switch (key)
	{
	case 0x0:   return a;   //a * 1.0f + b * 0.0f.
	case 0x1:   return -a;  //a * -1.0f + b * 0.0f.
	case 0x2:   return -b;  //a * 0.0f + b * -1.0f.
	case 0x3:   return b;   //a * 0.0f + b * 1.0f.
	};
	return 0.0f;
}

float PerlinNoise::GetPerlinNoise(float x, float y)
{
	//整数部と小数部に分ける.
	int xi = (int)floorf(x);
	int yi = (int)floorf(y);
	float xf = x - xi;
	float yf = y - yi;

	//格子点からハッシュを取り出し，その値を基に勾配を取得する.
	float a00 = Grad(GetHash(xi, yi), xf, yf);
	float a10 = Grad(GetHash(xi + 1, yi), xf - 1.0f, yf);
	float a01 = Grad(GetHash(xi, yi + 1), xf, yf - 1.0f);
	float a11 = Grad(GetHash(xi + 1, yi + 1), xf - 1.0f, yf - 1.0f);

	//補間をかける.
	xf = Fade(xf);
	yf = Fade(yf);

	//位置に合わせて格子点のどの点から一番影響を受けるかを決める.
	//(勾配関数内で内積を取っているので，ベクトルの向きによっては負の値が出る．範囲は-1.0f~1.0f).
	//(なので，正の値にするために1.0fを足して2.0fで割っている).
	return (Lerp(Lerp(a00, a10, xf), Lerp(a01, a11, xf), yf) + 1.0f) / 2.0f;
}

float PerlinNoise::OctavePerlinNoise(float x, float y)
{
	float a = 1.0f;
	float f = 1.0f;
	float maxValue = 0.0f;
	float totalValue = 0.0f;
	float per = 0.5f;
	for (int i = 0; i < 5; ++i) {
		totalValue += a * GetPerlinNoise(x * f, y * f);
		maxValue += a;
		a *= per;
		f *= 2.0f;
	}
	return totalValue / maxValue;
}