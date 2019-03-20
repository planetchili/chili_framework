#include "JC_Circle.h"
#include "JC_Math.h"
#include <math.h>

JC_Circle::JC_Circle(const JC_Point2d& P_in, const JC_Point2d& Q_in, Color color_in)
	:
	JC_Shape(color_in),
	O(P_in),
	radius(GetDistanceTo(P_in, Q_in))
	
{}


JC_Circle::JC_Circle(const JC_Point2d& P_in, const JC_Point2d& Q_in, const JC_Point2d& R_in, Color color_in)
	:
	JC_Shape(color_in),
	O(CalculateCentre(P_in, Q_in, R_in)),
	radius(GetDistanceTo(O, R_in))

{}


void JC_Circle::Draw(Camera cam_in)
{
	cam_in.DrawCircle(O, radius, 2, Base_Color);
}


bool JC_Circle::IsInRange(const JC_Point2d& mouse_in)
{
	double distance = GetDistanceTo(O, mouse_in);
	return (distance <= (radius + halfwidth) &&
		distance >= (radius - halfwidth));
}
