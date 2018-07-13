#include "Circle.h"
#include "JC_Math.h"
#include <math.h>

Circle::Circle(const JC_Point2d& P, const JC_Point2d& Q, Color color)
	:
	Shape(P,color),
	radius(GetDistanceTo(P, Q))
{}



Circle::Circle(const JC_Point2d& P, const JC_Point2d& Q, const JC_Point2d& R, Color color)
	:
	Shape(CalculatCentre(P, Q, R), color),
	radius(GetDistanceTo(Centre, R))
{}

void Circle::Draw(Camera cam)
{
	cam.DrawCircle(Centre, radius, C);
}



bool Circle::IsInRange(JC_Point2d mousein)
{
	double distance = GetDistanceTo(Centre, mousein);
	return (distance <= (radius + halfwidth) &&
		distance >= (radius - halfwidth));
}





JC_Point2d Circle::CalculatCentre(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R)
{
	{
		//when we have 2 flat lines in order  under right angle
		if (P.x == R.x && Q.y == R.y)
		{
			JC_Point2d C;
			C.x = (R.x + Q.x) / 2;
			C.y = (P.y + R.y) / 2;
			return C;

		}
		//when we have 2 flat lines in reverce order  under right angle
		else if (P.y == R.y && Q.x == R.x)
		{
			JC_Point2d C;
			C.x = R.x + P.x / 2;
			C.y = Q.y + R.y / 2;
			return C;
		}
		else if (P.x == R.x && Q.y != R.y)
		{

			JC_Point2d C = CalculateSpecificCentre(P, R, Q);
			return C;

		}
		else if (P.x != R.x && Q.y == R.y)
		{
			JC_Point2d C = CalculateSpecificCentre(R, Q, P);
			return C;
		}
		else //if (P.x != Q.x && P.y != Q.y || Q.x != R.x && Q.y != R.y)
		{
			JC_Point2d C = CalculateSpecificCentre(P, Q, R);
			return C;

		}
	}
}



JC_Point2d Circle::CalculateSpecificCentre(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R)
{
	{

		double m1 = LineSlopeBetween2Points(P, R);
		double m2 = LineSlopeBetween2Points(Q, R);

		//If slopes are the same lines are parallel ,do nothing (infinite radius)		
		assert(m2 - m1 != 0.0);

		// calculate perpendicular line slope...

		double m1_perp = InverceLineSlope(m1);
		double m2_perp = InverceLineSlope(m2);

		//mid point of orginally inserted lines		

		JC_Point2d mid_PR = GetMidPoint(P,R); 

		JC_Point2d mid_QR = GetMidPoint(Q,R);


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

		return C;
	}
}
