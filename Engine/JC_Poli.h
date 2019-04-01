#pragma once

#include "JC_Shape.h"

class JC_Poliline : public JC_Shape
{
public:
	JC_Poliline(std::vector<JC_Point2d> point_data, Color color_in = Colors::White)
		:
		PointData(point_data),
		JC_Shape(color_in)
	{}

	void Draw(Camera cam) override
	{
		cam.DrawPoliLine(PointData, Base_Color);
	}

	bool IsInRange(const JC_Point2d& M) override
	{
		JC_Point2d Current;
		JC_Point2d Previous;
		JC_Point2d C;
		for (int i = 1 ; i < PointData.size(); i++)
		{
			Current = PointData[i];
			Previous = PointData[i - 1];
			C = ClosestPoint(Previous, Current, M);

			if (IsBetween2Points(Previous, Current, C))
			{
				if ((abs(GetDistanceTo(M, C)) < halfwidth))
				{
					return true;
				}
			}
		}
		return false;
	}
	std::wstring MakeDescription() override
	{
		return	std::wstring(L"Not yet operatable");
	}

private:
	std::vector<JC_Point2d> PointData;
};