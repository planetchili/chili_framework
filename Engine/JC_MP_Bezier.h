#pragma once

#include "JC_Shape.h"




// Atempt to write multi point bezier curve



class JC_MP_Bezier : public JC_Shape
{
public:
	JC_MP_Bezier(std::vector<JC_Point2d> &line, Color color_in = Colors::White)
		:
		point_data(line),
		JC_Shape(color_in)
	{}

	void Draw(Camera cam) override
	{
		cam.DrawMPBezier(point_data, Base_Color);
	}
	bool IsInRange(const JC_Point2d& M) override
	{
		return false;
	}

	std::wstring MakeDescription() override
	{


		return	std::wstring(L"Not yet operatable");

	}

private:
	std::vector<JC_Point2d> point_data;
};