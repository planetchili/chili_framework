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

	void SetScale(double new_scale)
	{
		scale = new_scale;
	}

	double GetScale() const
	{
		return scale;
	}

	void DrawLine(JC_Point2d P_in, JC_Point2d Q_in, Color Color_in)
	{
		P_in -= Camera_Pos;
		Q_in -= Camera_Pos;
		ct.DrawLine(std::move(P_in), std::move(Q_in), std::move(Color_in));
	}
	void DrawPoliLine(std::vector<JC_Point2d> point_data, Color Color_in)
	{
		for (auto &P : point_data)
		{
			P -= Camera_Pos;
		}

		ct.DrawPoliLine(std::move(point_data), std::move(Color_in));
	}
	void DrawBezier(JC_Point2d P_in, JC_Point2d Q_in, JC_Point2d R_in, Color Color_in)
	{
		P_in -= Camera_Pos;
		Q_in -= Camera_Pos;
		R_in -= Camera_Pos;
		ct.DrawBezier(std::move(P_in), std::move(Q_in), std::move(R_in) ,std::move(Color_in));
	}

	void DrawMPBezier(std::vector<JC_Point2d> point_data, Color Color_in)
	{
		for (auto &P : point_data)
		{
			P -= Camera_Pos;
		}

		ct.DrawMPBezier(std::move(point_data), std::move(Color_in));
	}

	void DrawCircle(JC_Point2d pos, double radius, int t, Color Color_in)
	{
		pos -= Camera_Pos;
		ct.DrawCircle(std::move(pos), std::move(radius), t,  std::move(Color_in));
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
	
private:
	double scale = 1.0;
	JC_Point2d Camera_Pos = { 0.0,0.0 };
	CoordinateTrasformer& ct;

};