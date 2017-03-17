#include "RectF.h"

RectF::RectF(float inLeft, float inRight, float inTop, float inBottom) :
	left(inLeft),
	right(inRight),
	top(inTop),
	bottom(inBottom)
{
}

RectF::RectF(const Vec2& recTop, const Vec2& recBottom) :
	RectF(recTop.x, recBottom.x, recTop.y, recBottom.y)
{
}

RectF::RectF(const Vec2& recTop, float recWidth, float recHeight) :
	RectF(recTop, recTop + Vec2(recWidth, recWidth))
{
}

RectF RectF::FromCenter(const Vec2& center, float halfWidth, float halfHeight)
{
	const Vec2 half(halfWidth, halfHeight);
	return RectF(center - half, center + half);
}

bool RectF::isOVerlapping(const RectF& rect) const
{
	return right > rect.left && left < rect.right && top < rect.bottom && bottom > rect.top;
}
