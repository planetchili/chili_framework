#include "Circle.h"
#include "JC_Math.h"
#include <math.h>

Circle::Circle(const JC_Point2d& P, const JC_Point2d& Q)
{
	pos = P;
	radius = CalculateRadius(P,Q);
}

Circle::Circle(const JC_Point2d& P, const JC_Point2d& Q, const JC_Point2d& R)
{
	// find slope of the line created by 2 points
	double m1 = 0.0f;
	double m2 = 0.0f;
	//when we have 2 flat lines in order  under right angle
	if (P.x == R.x && Q.y == R.y)
	{
		JC_Point2d C;
		C.x = (R.x + Q.x) / 2;
		C.y = (P.y + R.y) / 2;
		pos = C;
		radius = CalculateRadius(C, R);

	}
	//when we have 2 flat lines in reverce order  under right angle
	else if (P.y == R.y && Q.x == R.x)
	{
		JC_Point2d C;
		C.x = R.x + P.x / 2;
		C.y = Q.y + R.y / 2;
		pos = C;
		radius = CalculateRadius(C, R);
	}
	else if (P.x != Q.x && P.y != Q.y || Q.x != R.x && Q.y != R.y)
	{
		m1 = LineSlopeBetween2Points(P,R);
		m2 = LineSlopeBetween2Points(Q,R);

		//If slopes are the same lines are parallel ,do nothing (infinite radius)		
		if (m2 - m1 == 0.0)
		{
			
		}
		else
		{
			// calculate perpendicular line slope...

			double m1_perp = InverceLineSlope(m1);
			double m2_perp = InverceLineSlope(m2);

			//mid point of orginally inserted lines		

			JC_Point2d mid_PR = P.GetMidPoint(R);

			JC_Point2d mid_QR = Q.GetMidPoint(R);


			// y=mx+b find b part of perpendicular line
			// y-y1 = m(x-x1)  = > y = mx - m*x1 + y1
			//https://www.varsitytutors.com/hotmath/hotmath_help/topics/point-slope-form.html

			double a = (-m1_perp * mid_PR.x + mid_PR.y);
			double b = (-m2_perp * mid_QR.x + mid_QR.y);

			// find crossection
			//http://www.ambrsoft.com/MathCalc/Line/TwoLinesIntersection/TwoLinesIntersection.htm

			JC_Point2d C;
			C.x = (a - b) / (m2_perp - m1_perp);
			C.y = ((m1_perp * b) - (m2_perp * a)) / (m1_perp - m2_perp);

			//Calculate final circle
			pos = C;
			radius = CalculateRadius(C,R);
		}
	}
}
