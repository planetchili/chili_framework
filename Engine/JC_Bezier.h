#pragma once

#include "JC_Shape.h"
#include "Camera.h"

class JC_Bezier : public JC_Shape
{
public:
	JC_Bezier(JC_Point2d P, JC_Point2d Q, JC_Point2d R,Color color_in = Colors::White)
		:
		P(P),
		Q(Q),
		R(R),
		JC_Shape(color_in)
	{}

	void Draw(Camera cam) override
	{
		cam.DrawBezier(P, Q, R, Base_Color);
	}
	bool IsInRange(const JC_Point2d& M) override
	{
		return false;
	}

	std::wstring MakeDescription() override
	{

		std::wstring PosP = L"X: " + std::to_wstring(P.x) + L"\n Y: " + std::to_wstring(P.y);
		std::wstring PosQ = L"X: " + std::to_wstring(Q.x) + L"\n Y: " + std::to_wstring(Q.y);
		std::wstring PosR = L"X: " + std::to_wstring(R.x) + L"\n Y: " + std::to_wstring(R.y);

		return	std::wstring(L"First point:\n " + PosP + L"Second point:\n " + PosQ + L"Third point:\n " + PosR);

	}

private:
	JC_Point2d P, Q, R;
	
};