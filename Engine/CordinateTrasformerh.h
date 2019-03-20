#pragma once

#include <vector>

#include "Graphics.h"
#include "Colors.h"
#include "JC_Vector2.h"


class CoordinateTrasformer
{
public:

	CoordinateTrasformer (Graphics& gfx)
		:
		gfx(gfx)
	{}

	void DrawLine(JC_Point2d P_in, JC_Point2d Q_in, Color Color_in)
	{
		JC_Vector2d offset = { double(Graphics::ScreenWidth / 2),double(Graphics::ScreenHeight / 2) };

		P_in.y *= -1;
		Q_in.y *= -1;

		P_in += offset;
		Q_in += offset;


		gfx.DrawLine(P_in, Q_in, Color_in);
	}

	void DrawCircle(JC_Point2d pos, double radius, int t, Color c)
	{
		JC_Vector2d offset = { double(Graphics::ScreenWidth / 2),double(Graphics::ScreenHeight / 2) };

		pos.y *= -1.0;
		pos += offset;

		gfx.DrawCircle(pos, radius, t, c);
	}

private:
	Graphics& gfx;



};