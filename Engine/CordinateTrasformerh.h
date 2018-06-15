#pragma once

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

	void DrawCircle(JC_Point2d pos, double radius, Color c)
	{
		JC_Vector2d offset = { double(Graphics::ScreenWidth / 2),double(Graphics::ScreenHeight / 2) };

		pos.y *= -1.0;
		pos += offset;

		gfx.DrawCircle(pos, radius, c);
	}

private:
	Graphics& gfx;



};