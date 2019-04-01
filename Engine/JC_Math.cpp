#include "JC_Math.h"


int CalcFactorial(int n)
{
	if (n > 1)
	return n * CalcFactorial(n - 1);
	else
	return 1;
}

int Binomial_Make(int N, int K)
{
	int f_N = CalcFactorial(N);
	int f_K = CalcFactorial(K);
	int f_NK = CalcFactorial(N - K);

	return (f_N / (f_K * f_NK));
}

JC_Point2d CalculateCentre(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R)
{
	//when we have 2 flat lines in order under right angle
	if (std::abs(P.x) == std::abs(R.x) && std::abs(Q.y) == std::abs(R.y))
	{
		JC_Point2d C;
		C.x = (R.x + P.x) / 2;
		C.y = (Q.y + R.y) / 2;
		return C;
	}
	//when we have 2 flat lines in reverce order under right angle
	else if (std::abs(P.y) == std::abs(R.y) && std::abs(Q.x) == std::abs(R.x))
	{
		JC_Point2d C;
		C.x = R.x + Q.x / 2;
		C.y = P.y + R.y / 2;
		return C;
	}
	else if (std::abs(P.x) == std::abs(R.x) && std::abs(Q.y) != std::abs(R.y) || std::abs(P.y) == std::abs(R.y) && std::abs(Q.x) != std::abs(R.x))
	{
		JC_Point2d C = CalculateSpecificCentre(P, R, Q);
		return C;
	}
	else if (std::abs(P.x) != std::abs(R.x) && std::abs(Q.y) == std::abs(R.y) || std::abs(P.y) == std::abs(R.y) && std::abs(Q.x) != std::abs(R.x))
	{
		JC_Point2d C = CalculateSpecificCentre(R, Q, P);
		return C;
	}
	else 
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

JC_Point2d ClosestPoint(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R)
{
	const double a = P.y - Q.y;
	const double b = Q.x - P.x;
	const double c = P.x * Q.y - Q.x * P.y;

	JC_Point2d C;

	C.x = (b * (b * R.x - (a * R.y)) - (a * c)) / (Square(a) + Square(b));
	C.y = (a * ((-b * R.x )+ ( a * R.y)) - (b * c)) / (Square(a) + Square(b));


	return C;
}
	


bool IsBetween2Points(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R)
{
	JC_Point2d C = ClosestPoint(P, Q, R);

	if ((GetDistanceTo(P, C) < GetDistanceTo(P, Q)) && (GetDistanceTo(Q, C) < GetDistanceTo(P, Q)))
		return true;
	else
		return false;

}
