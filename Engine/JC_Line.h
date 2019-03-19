#pragma once

#include "JC_Shape.h"
#include "Camera.h"

class JC_Line : public JC_Shape
{
public:
	JC_Line(const JC_Point2d& P_in, const JC_Point2d& Q_in, Color color_in = Colors::White)
		:
		P(P_in),
		Q(Q_in),
		JC_Shape(color_in)
	{}

	void Draw(Camera cam) override
	{
		cam.DrawLine(P, Q, Base_Color);
	}
	bool IsInRange(const JC_Point2d& M) override
	{
		JC_Point2d C = ClosesPoint(P, Q, M);

		if (IsBetween2Points(P, Q, C))
		{
			if ((abs(GetDistanceTo(M,C)) < halfwidth))
				return true;

			else
				return false;
		}
		else
			return false;
	}

	std::wstring MakeDescription() override
	{
		
		std::wstring PosP = std::to_wstring(P.x) + L" " + std::to_wstring(P.y);
		std::wstring PosQ = std::to_wstring(Q.x) + L" " + std::to_wstring(Q.y);
		std::wstring Lenght = std::to_wstring(GetDistanceTo(P,Q));

		return	std::wstring(L"Start:\n" + PosP + L"\n\n" + L" End \n " + PosQ + L"\n\n" + L"Lenght: " + Lenght);
			
	}

private:
	JC_Point2d P, Q; //Start and end points of the line

};