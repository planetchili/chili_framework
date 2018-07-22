#pragma once


#include "JC_Shape.h"
#include "JC_Math.h"

#include "JC_Vector2.h"
#include "Graphics.h"

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
	JC_Point2d O;
	double radius;

};