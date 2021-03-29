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
#include "MainWindow.h"
#include "Game.h"

#include<iostream>
#include "Mat3.h"

#include "Log.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	cub(0.5f),
	m_checkerboardTexture(Surface::FromFile(L"checkerboardPattern.bmp")),
	m_diceTexture(Surface::FromFile(L"diceUV.bmp"))
{}

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
	texturedVertex t1;
	t1.m_position = Vec3(wnd.mouse.GetPosX(), wnd.mouse.GetPosY(), 0.0f);
	//t1.m_position = Vec3(250.0f,50.0f , 0.0f);
	//t1.m_position = Vec3(250.0f, 500.0f, 0.0f);
	t1.m_uv_coordinates = Vec2( 0.5f,1.0f );

	texturedVertex t2;
	t2.m_position = Vec3( 0.0f,250.0f,0.0f );
	t2.m_uv_coordinates = Vec2( 0.0f,0.0f );

	texturedVertex t3;
	t3.m_position = Vec3( 500.0f,250.0f,0.0f );
	t3.m_uv_coordinates = Vec2( 1.0f,0.0f );


	gfx.DrawTriangle(t1, t2, t3, m_checkerboardTexture);
}