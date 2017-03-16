#pragma once
#include "Vec2.h"

class RectF
{
public:
	RectF() = default;
	RectF(float inLeft, float inRight, float inTop, float inBottom);
	RectF(const Vec2& recTop, const Vec2& recBottom);
	RectF(const Vec2& recTop, float recWidth, float recHeight);
	static RectF FromCenter(const Vec2& center, float halfWidth, float halfHeight);
	bool isOVerlapping(const RectF& rect) const;
public:
	float left;
	float right;
	float top;
	float bottom;
};