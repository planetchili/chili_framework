#pragma once


#include "JC_Shape.h"
#include "Graphics.h"
#include "JC_Vector2.h"
#include "Camera.h"


class JC_Circle : public JC_Shape
{
public:

	JC_Circle(const JC_Point2d& P, const JC_Point2d& Q,Color color = Colors::White);
	JC_Circle(const JC_Point2d& P, const JC_Point2d& Q, const JC_Point2d& R, Color color = Colors::White);
	
	void Draw(Camera cam) override;
	bool IsInRange(JC_Point2d mousein) override;
		
private:
	JC_Point2d CalculatCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	JC_Point2d CalculateSpecificCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	
private:
	double radius;

};