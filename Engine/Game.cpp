/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
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
#include "Game.h"
#include<iostream>
#include "log/Log.h"

#include "dataStructure/math/Mat3.h"
#include "dataStructure/math/Vec3.h"

#include "effects/textureEffect.h"

#include "graphics/MainWindow.h"


Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	cub(0.5f)
{
	//setup the pipeline properly as the first order of business..
	pipeline<textureEffect>::gfx = &gfx;
	pipeline<textureEffect>::bindTexture(std::filesystem::path(L"resources/texture/diceUV.bmp"));
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{}
void Game::ComposeFrame()
{
	static Mat3 rotation = Mat3::Identity();

	if (wnd.kbd.KeyIsPressed(0x5A))	{ rotation = rotation * Mat3::RotateZ(0.0174533); }
	if (wnd.kbd.KeyIsPressed(0x59)) { rotation = rotation * Mat3::RotateY(0.0174533); }
	if (wnd.kbd.KeyIsPressed(0x58)) { rotation = rotation * Mat3::RotateX(0.0174533); }

	pipeline<textureEffect>::translate(Vec3(0.0f, 0.0f, 2.0f)); //push the cube model by two in the z
	pipeline<textureEffect>::bindRotationMatrix(rotation);
	pipeline<textureEffect>::draw(cub.getIndexBuffer(), cub.getVertexBuffer());
}