#pragma once

#include "Graphics.h"
#include "JC_Vector2.h"

class Circle
{
public:

	Circle(const JC_Point2d& P, const JC_Point2d& Q,Color color = Colors::White);
	Circle(const JC_Point2d& P, const JC_Point2d& Q, const JC_Point2d& R, Color color = Colors::White);
	
	void Draw(Graphics& gfx);

	//void CalculateSelectCircle(const JC_Point2i& mouse_imput);

public:
	JC_Point2d CalculatCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	
private:
	JC_Point2d CalculateSpecificCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	//Circle GetCircle();
private:
	JC_Point2d pos;
	double radius;
	static constexpr double halfwidth = 4.0;
	//bool selected = false;
	Color C ;
};

