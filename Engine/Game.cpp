/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
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
#include "JezierVec2.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	xpos(720),
	ypos(450),
	x1(100),  y1(450), x2(1300), y2(300),
	c(Colors::White)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (wnd.mouse.LeftIsPressed())
	{
		xpos = wnd.mouse.GetPosX();
		ypos = wnd.mouse.GetPosY();
		c = Colors::Yellow;
	}
	else
		c = Colors::White;
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		y2--;
		y1--;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		y2++;
		y1++;
	}
}

void Game::ComposeFrame()
{

	gfx.DrawLine(x1, y1, x2, y2, Colors::White);
	gfx.DrawCircle(xpos, ypos, 100, c);
	gfx.DrawArc(520,450,720,450,45,120, Colors::White);

}
