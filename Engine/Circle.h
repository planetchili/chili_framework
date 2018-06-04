#pragma once

#include "Graphics.h"
#include "Mouse.h"
#include "Colors.h"
#include "JC_Vector2.h"

class Circle
{
public:

	Circle(const JC_Point2d& P, const JC_Point2d& Q);

	Circle(const JC_Point2d& P, const JC_Point2d& Q, const JC_Point2d& R);
			

	void Draw(Graphics& gfx )
	{
		gfx.DrawCircle(pos, radius, C);
	}

	//Circle CalculateSelect();


private:
	JC_Point2d pos;
	double radius;
	static constexpr double halfwidth = 4.0;
	Color C = Colors::White;
};

