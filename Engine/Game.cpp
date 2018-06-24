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
#include "Mouse.h"
#include "CordinateTrasformerh.h"


Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ct(gfx)
{
	//circles.emplace_back(JC_Point2d{ 0,0 }, JC_Point2d{ 300,300 });
	//circles.emplace_back(JC_Point2d{ 100,0 }, JC_Point2d{ 0,100 },JC_Point2d{ 0,0 });
	//circles.emplace_back(JC_Point2d{ 200,200 }, JC_Point2d{ 300,300 }, JC_Point2d{ 400,450 });


	//circles.emplace_back(JC_Point2d{ -100,0 }, JC_Point2d{ 100,0 }, JC_Point2d{ 0,100 },Colors::Magenta);
	
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
	switch (wnd.shape)
	{
	case MainWindow::Shape::TwoPointCircle :
		{
			while (!wnd.mouse.IsEmpty())
			{
				const auto e = wnd.mouse.Read();

				if (e.GetType() == Mouse::Event::Type::LPress)
				{
					if (input == 0)
					{
						P.x = (double)wnd.mouse.GetPosX();
						P.y = (double)wnd.mouse.GetPosY();
						engaged = true;
						P = ct.CreatePoint(P);
					}
					if (input == 1)
					{
						Q.x = (double)wnd.mouse.GetPosX();
						Q.y = (double)wnd.mouse.GetPosY();
						Q = ct.CreatePoint(Q);
						circles.emplace_back(P, Q);
					}

					input++;

					if (input >= 2)
					{
						input = 0;
						engaged = false;
					}


				}
				if (e.GetType() == Mouse::Event::Type::RPress)
				{
					input = 0;
					engaged = false;
				}

			}
			if (engaged)
			{
				Q.x = (double)wnd.mouse.GetPosX();
				Q.y = (double)wnd.mouse.GetPosY();
				Q = ct.CreatePoint(Q);
				ct.DrawCircle(P, GetDistanceTo(P, Q), Colors::Red);
			}
			break;
		}
		break;
	}

	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		input = 0;
		wnd.shape = MainWindow::Shape::Null;
	}
	

}

void Game::ComposeFrame()
{
	for (auto c : circles)
	{
		c.Draw(ct);
	}

	ct.DrawClosedPolyline(Star::Make(200, 75.0,7), Colors::Red);
}

