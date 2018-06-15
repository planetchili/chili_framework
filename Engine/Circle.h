#pragma once

#include "Graphics.h"
#include "JC_Vector2.h"
#include "CordinateTrasformerh.h"

class Circle
{
public:

	Circle(const JC_Point2d& P, const JC_Point2d& Q,Color color = Colors::White);
	Circle(const JC_Point2d& P, const JC_Point2d& Q, const JC_Point2d& R, Color color = Colors::White);
	
	void Draw(CoordinateTrasformer& ct);

	//void CalculateSelectCircle(const JC_Point2i& mouse_imput);
	Circle GetCircle();

public:
	JC_Point2d CalculatCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	double GetRadius()
	{
		return radius;
	}
	JC_Point2d GetPos()
	{
		return pos;
	}
private:
	JC_Point2d CalculateSpecificCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	
private:
	JC_Point2d pos;
	double radius;
	static constexpr double halfwidth = 4.0;
	bool selected = false;
	Color C ;
};

