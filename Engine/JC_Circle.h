#pragma once


#include "JC_Shape.h"


#include "Graphics.h"
#include "JC_Vector2.h"
#include "Camera.h"


class JC_Circle : public JC_Shape
{
public:

	JC_Circle(const JC_Point2d& P_in, const JC_Point2d& Q_in, Color color_in = Colors::White);

	//Constructor which is responsible for creating circle using three points
	JC_Circle(const JC_Point2d& P_in, const JC_Point2d& Q_in, const JC_Point2d& R_in, Color color_in = Colors::White);
	
	void Draw(Camera cam_in) override;
	bool IsInRange(const JC_Point2d& mouse_in) override;
		
private:
	//**We have couple of types of centre calculation:
	//**When we can Determine slope of both lines, then we proces points in seen order.
	//**When we can't determine slope of any of lines (parallel to the window sides) 
	//   in this situation we are calculating centre using line mid point rule
	//**When we are unable to tetermine solope of one from the lines
	//   in this situation we are simply swaping order of the points.
	JC_Point2d CalculatCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);

	//Used to calculate centre from points from which created lines have caclulatable slope (at least one line)
	JC_Point2d CalculateSpecificCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	
private:
	JC_Point2d O;
	double radius;

};