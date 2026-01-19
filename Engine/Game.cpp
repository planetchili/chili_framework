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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	brd(20, 20),
	player({2, 1})
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
	if (wnd.kbd.KeyIsPressed(VK_RIGHT) && !rightPressed)
	{	
		rightPressed = true;
		playerDelta_Loc = { 1, 0 };
		player.MoveBy(playerDelta_Loc);
	}
	if (wnd.kbd.KeyIsPressed(VK_LEFT) && !leftPressed)
	{	
		leftPressed = true;
		playerDelta_Loc = { -1, 0 };
		player.MoveBy(playerDelta_Loc);
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN) && !downPressed)
	{	
		downPressed = true;
		playerDelta_Loc = { 0, 1 };
		player.MoveBy(playerDelta_Loc);
	}
	if (wnd.kbd.KeyIsPressed(VK_UP) && !upPressed)
	{	
		upPressed = true;
		playerDelta_Loc = { 0, -1 };
		player.MoveBy(playerDelta_Loc);
	}
}

void Game::ComposeFrame()
{
	player.Draw(brd, gfx, Colors::Green);
}