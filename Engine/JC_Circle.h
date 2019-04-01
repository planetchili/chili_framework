#pragma once


#include "JC_Shape.h"





class JC_Circle : public JC_Shape
{
public:

	JC_Circle(const JC_Point2d& P_in, const JC_Point2d& Q_in, Color color_in = Colors::White);

	//Constructor which is responsible for creating circle using three points
	JC_Circle(const JC_Point2d& P_in, const JC_Point2d& Q_in, const JC_Point2d& R_in, Color color_in = Colors::White);
	
	void Draw(Camera cam_in) override;
	bool IsInRange(const JC_Point2d& mouse_in) override;
		

	std::wstring MakeDescription() override
	{

		std::wstring PosO = L"X: " + std::to_wstring(O.x) + L"\n Y: " + std::to_wstring(O.y);
		std::wstring Radius = std::to_wstring(radius);
		std::wstring Diameter = std::to_wstring(radius*2 * PI_D);
		std::wstring Area = std::to_wstring( radius * radius * PI_D);

		return	std::wstring(L"Centre:\n " + PosO + L"\n\nRadius: " + Radius + L"\n\nDiameter: " + Diameter + L"\n\nArea: " + Area);

	}

private:
	JC_Point2d O;
	double radius;


};