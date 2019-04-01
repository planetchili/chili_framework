#pragma once

#include "JC_Vector2.h"

template<typename T>
class CRectangle
{
public:
	CRectangle(T left_in, T right_in, T top_in, T bottom_in)
		:
		left(left_in),
		right(right_in),
		top(top_in),
		bottom(bottom_in)
	{}
	CRectangle(const JC_Vector2<T>& topLeft, const  JC_Vector2<T>& bottomRight)
		:
		CRectangle(topLeft.x, bottomRight.x, topLeft.y, bottomRight.y)
	{}
	CRectangle(const JC_Vector2<T>& topLeft, T width, T height)
		:
		CRectangle(topLeft, topLeft + JC_Vector2<T>(width, height))
	{}
	bool IsOverlappingWith(const CRectangle& other) const
	{
		return right > other.left && left < other.right
			&& bottom > other.top && top < other.bottom;
	}
	bool IsContainedBy(const CRectangle& other) const
	{
		return left >= other.left && right <= other.right &&
			top >= other.top && bottom <= other.bottom;
	}
	bool Contains(const  JC_Vector2<T>& point) const
	{
		return point.x >= left && point.x < right && point.y >= top && point.y < bottom;
	}
	CRectangle FromCenter(const JC_Vector2<T>& center, T halfWidth, T halfHeight)
	{
		const JC_Vector2<T> half(halfWidth, halfHeight);
		return CRectangle(center - half, center + half);
	}
	CRectangle GetExpanded(T offset) const
	{
		return CRectangle(left - offset, right + offset, top - offset, bottom + offset);
	}
	JC_Vector2<T> GetCenter() const
	{
		return JC_Vector2<T>((left + right) / (T)2, (top + bottom) / (T)2);
	}
	T GetWidth() const
	{
		return right - left;
	}
	T GetHeight() const
	{
		return bottom - top;
	}
public:
	T left;
	T right;
	T top;
	T bottom;
};

typedef CRectangle<int> RectI;
typedef CRectangle<double> RectD;