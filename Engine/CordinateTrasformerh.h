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


	void DrawPoliLine(std::vector<JC_Point2d> point_data, Color Color_in)
	{
		JC_Vector2d offset = { double(Graphics::ScreenWidth / 2),double(Graphics::ScreenHeight / 2) };

		for (auto &P : point_data)
		{
			P.y *= -1;
			P += offset;
		}

		gfx.DrawPoliLine(point_data, Color_in);
	}


	void DrawBezier(JC_Point2d P_in, JC_Point2d Q_in, JC_Point2d R_in, Color Color_in)
	{
		JC_Vector2d offset = { double(Graphics::ScreenWidth / 2),double(Graphics::ScreenHeight / 2) };

		P_in.y *= -1;
		Q_in.y *= -1;
		R_in.y *= -1;

		P_in += offset;
		Q_in += offset;
		R_in += offset;

		gfx.DrawBezier(P_in, Q_in, R_in, Color_in);
	}


	void DrawMPBezier(std::vector<JC_Point2d> point_data, Color Color_in)
	{
		JC_Vector2d offset = { double(Graphics::ScreenWidth / 2),double(Graphics::ScreenHeight / 2) };

		for (auto &P : point_data)
		{
			P.y *= -1;
			P += offset;
		}
		
		gfx.DrawBezier(point_data, Color_in);
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