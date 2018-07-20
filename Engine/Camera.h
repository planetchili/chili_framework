#pragma once

#include "CordinateTrasformerh.h"
#include "JC_Vector2.h"

//Camera class should be responsible for scaling onad trafromation of the drawn objects
//It is the part of the screen pipeline

class Camera
{
public:
	Camera (CoordinateTrasformer& ct)
		:
		ct(ct)
	{}

	JC_Point2d GetPos()const
	{
		return Camera_Pos;
	}

	void MoveBy(const JC_Vector2d& offset)
	{
		Camera_Pos += offset;
	}

	void MoweTo(const JC_Point2d pos_in)
	{
		Camera_Pos = pos_in;
	}

	void DrawLine(JC_Point2d P_in, JC_Point2d Q_in, Color Color_in)
	{
		P_in -= Camera_Pos;
		Q_in -= Camera_Pos;
		ct.DrawLine(std::move(P_in), std::move(Q_in), std::move(Color_in));
	}
	
	void DrawCircle(JC_Point2d pos, double radius, Color Color_in)
	{
		pos -= Camera_Pos;
		ct.DrawCircle(std::move(pos), std::move(radius), Color_in);
	}
	
	void DrawClosedPolyline(std::vector<JC_Point2d> poly, Color c)
	{
		for (auto& v : poly)
		{
			v -= Camera_Pos;
		}
		ct.DrawClosedPolyline(std::move(poly), c);
	}

	// trasforms Point screen coordinates to mathematical coordinates regardles from cmaera position

	template <typename T> JC_Point2<T> TrasformPoint(JC_Point2<T> input)
	{
		//fixes disconection between screen and math coordinates
		input.x += (T)(Camera_Pos.x); 
		input.y -= (T)(Camera_Pos.y);

		JC_Vector2<T> offset = { (T)(Graphics::ScreenWidth / 2), (T)(Graphics::ScreenHeight / 2) };
		input -= offset;
		input.y *= -1;
		return input;
	}

	void SetScale(double new_scale)
	{
		scale = new_scale;
	}

	double GetScale() const
	{
		return scale;
	}

private:
	double scale = 1.0;
	JC_Point2d Camera_Pos = { 0.0,0.0 };
	CoordinateTrasformer& ct;

};