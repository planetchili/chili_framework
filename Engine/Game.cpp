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
#include "JC_Vector2.h"
#include "Mouse.h"


Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ct(gfx)
{
	circles.emplace_back(JC_Point2d{ 0,0 }, JC_Point2d{ 300,300 });
	circles.emplace_back(JC_Point2d{ 600,450 }, JC_Point2d{ 840,450 },JC_Point2d{ 720,330 });
	circles.emplace_back(JC_Point2d{ 200,200 }, JC_Point2d{ 300,300 }, JC_Point2d{ 400,450 });
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
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			input++;

			if (input >= 2)
				input = 0;
		}
	}



}

void Game::ComposeFrame()
{
	for (auto c : circles)
	{
		c.Draw(ct);
	}
	
}
