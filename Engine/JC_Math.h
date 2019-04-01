#pragma once

#include "JC_Vector2.h"
#include <assert.h>
#include <algorithm>

constexpr double PI_F = 3.1415926536;
constexpr double PI_D = 3.1415926535897932;



	template <typename T>
	inline auto Square(const T& x)
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

		//assert(dx != 0.0);

		double m = dy / dx;

		return m;

	}

	inline double InverceLineSlope(const double& m)
	{
		//assert(m != 0.0f);

		return -1 / m;
	}


	int CalcFactorial(int n);
	
	int Binomial_Make(int N, int K);



	// We have couple of types of centre calculation:
	// When we can Determine slope of both lines, then we proces points in seen order.
	// When we can't determine slope of any of lines (parallel to the window sides) 
	//   in this situation we are calculating centre using line mid point rule
	// When we are unable to determine solope of one the lines
	//   in this situation we are simply swaping order of the points.
	JC_Point2d CalculateCentre(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R);
	

	//Used to calculate centre from points from which created lines have caclulatable slope (at least one line)
	JC_Point2d CalculateSpecificCentre(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R);

	// First two points create line third we are exsamining
	JC_Point2d ClosestPoint(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R);




	bool IsBetween2Points(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R);

	
	


