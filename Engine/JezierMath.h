#pragma once

#include "math.h"
#include <type_traits>

constexpr double PI_F = 3.1415926536;
constexpr double PI_D = 3.1415926535897932;


template <typename T>
inline auto Sq(const T& x)
{
	return x * x;
}

//TO DO
// function two points as parameter retuens slope of the coresponding line

inline float Slope(const float x1, const float y1, const float x2, const float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	if (abs(dx) > abs(dy))
	{
		float m = dy / dx;
		return m;
	}
	else
	{

		float m = dx / dy;
		return m;
	}
}

