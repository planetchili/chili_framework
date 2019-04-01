/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include <memory>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "CordinateTrasformerh.h"
#include "Camera.h"
#include "MouseCameraControler.h"

#include "JC_Shape.h"
#include "JC_Vector2.h"
#include "JC_Math.h"
#include "JC_Circle.h"
#include "JC_Line.h"
#include "JC_Poli.h"
#include "ALIB_Bezier.h"


class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();



private:
	void ComposeFrame();
	void UpdateModel();
	void ProcesInput();
	/********************************/
	/*  User Functions              */



	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	CoordinateTrasformer ct;
	Camera cam;
	//MouseCameraController camCtrl;
	std::vector<std::unique_ptr<JC_Shape>> Shapes;
	unsigned short input = 0;

	bool first_point_engagement = false;
	bool second_point_engagement = false;
	   
	JC_Point2d P, Q, R;
	std::vector<JC_Point2d> point_data;
	
	/********************************/

};


//enum class GameCreationState
	//{
	//	Null,
	//	FirstPoint,
	//	SecondPoint,
	//	ThirdPoint,
	//	NextPoint,
	//	Count
	//};
	//GameCreationState CreoState = GameCreationState::Null;