#include "JC_Math.h"

JC_Point2d CalculateCentre(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R)
{
	
	//when we have 2 flat lines in order under right angle
	if (std::abs(P.x) == std::abs(R.x) && std::abs(Q.y) == std::abs(R.y))
	{
		JC_Point2d C;
		C.x = (R.x + Q.x) / 2;
		C.y = (P.y + R.y) / 2;
		return C;

	}
	//when we have 2 flat lines in reverce order under right angle
	else if (std::abs(P.y) == std::abs(R.y) && std::abs(Q.x) == std::abs(R.x))
	{
		JC_Point2d C;
		C.x = R.x + P.x / 2;
		C.y = Q.y + R.y / 2;
		return C;
	}
	else if (std::abs(P.x) == std::abs(R.x) && std::abs(Q.y) != std::abs(R.y))
	{

		JC_Point2d C = CalculateSpecificCentre(P, R, Q);
		return C;

	}
	else if (std::abs(P.x) != std::abs(R.x) && std::abs(Q.y) == std::abs(R.y))
	{
		JC_Point2d C = CalculateSpecificCentre(R, Q, P);
		return C;
	}
	else if (std::abs (P.x) != std::abs(Q.x) && std::abs(P.y) != std::abs(Q.y) || std::abs(Q.x) != std::abs(R.x) && std::abs(Q.y) != std::abs(R.y))
	{
		JC_Point2d C = CalculateSpecificCentre(P, Q, R);
		return C;

	}

}

JC_Point2d CalculateSpecificCentre(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R)
{


	double m1 = LineSlopeBetween2Points(P, R);
	double m2 = LineSlopeBetween2Points(Q, R);

	//If slopes are the same lines are parallel ,do nothing (infinite radius)		
	assert(m2 - m1 != 0.0);

	// calculate perpendicular line slope...

	double m1_perp = InverceLineSlope(m1);
	double m2_perp = InverceLineSlope(m2);

	//mid point of orginally inserted lines		

	JC_Point2d mid_PR = GetMidPoint(P, R);

	JC_Point2d mid_QR = GetMidPoint(Q, R);


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
