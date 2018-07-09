#pragma once

#include "Graphics.h"
#include "JC_Vector2.h"
#include "Camera.h"


class Circle
{
public:

	Circle(const JC_Point2d& P, const JC_Point2d& Q,Color color = Colors::White);
	Circle(const JC_Point2d& P, const JC_Point2d& Q, const JC_Point2d& R, Color color = Colors::White);
	
	void Draw(Camera cam);
	void UpdateColor();
	
	void SetTrueSelectionFlag(const JC_Point2d& mousein);
	void ResetSelectionFlag();

	bool ReadyForRemoval() const;


private:
	JC_Point2d CalculatCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	JC_Point2d CalculateSpecificCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	
private:
	JC_Point2d pos;
	double radius;
	static constexpr double halfwidth = 10.0;
	bool selectedflag = false;
	Color C ;
};