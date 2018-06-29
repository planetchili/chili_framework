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
	void UpdateColor();
	
	void SetSelectionFlag(const JC_Point2i& mousein);
	void ResetSelectionFlag();

	bool ReadyForRemoval() const;

public:
	JC_Point2d CalculatCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
			

private:
	JC_Point2d CalculateSpecificCentre(const  JC_Point2d& P, const  JC_Point2d& Q, const JC_Point2d& R);
	
private:
	JC_Point2d pos;
	double radius;
	static constexpr double halfwidth = 4.0;
	bool selectedflag = false;
	Color C ;
};

//	if (flag == true)
//{
//	
//}