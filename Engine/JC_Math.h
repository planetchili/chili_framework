#pragma once

#include "JC_Vector2.h"
#include <assert.h>


constexpr double PI_F = 3.1415926536;
constexpr double PI_D = 3.1415926535897932;


template <typename T>
inline auto Sq(const T& x)
{
	return x * x;
}


inline float Slope(const float x1, const float y1, const float x2, const float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	float m = dy / dx;
	return m;
	
}

inline double LineSlopeBetween2Points(const  JC_Point2d& P, const JC_Point2d& Q) 
{
	double dx = Q.x - P.x;
	double dy = Q.y - P.y;

	assert(dx != 0.0);

	double m = dy / dx;

	return m;

}

inline double InverceLineSlope(const double& m) 
{
	assert(m != 0.0f);

	return -1 / m;
}



