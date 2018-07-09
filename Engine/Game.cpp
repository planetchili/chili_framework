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
#include "ChiliUtil.h"

#include <functional>

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ct(gfx),
	cam(ct),
	camCtrl(wnd.mouse,cam)
{	
}

void Game::Go()
{
	gfx.BeginFrame();	
	ProcesInput();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcesInput()
{
	switch (wnd.shape)
	{
	case MainWindow::MWShapeState::TwoPointCircle:
	{
		while (!wnd.mouse.IsEmpty())
		{
			const auto e = wnd.mouse.Read();

			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				if (input == 0)
				{

					P = wnd.mouse.GetPos();
					engaged = true;
					P = cam.TrasformPoint(P);
				}
				if (input == 1)
				{

					Q = wnd.mouse.GetPos();
					Q = cam.TrasformPoint(Q);
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
			Q = wnd.mouse.GetPos();
			Q = cam.TrasformPoint(Q);
			cam.DrawCircle(P, GetDistanceTo(P, Q), Colors::Red);
		}
		break;
	}
	case MainWindow::MWShapeState::Null:
	{
		while (!wnd.mouse.IsEmpty())
		{
			const auto e = wnd.mouse.Read();

			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				for (auto i = circles.begin(), j = circles.end(); i != j; ++i)
				{
					if (wnd.kbd.KeyIsPressed(VK_SHIFT))
					{
						if (i->IsInRange(static_cast<JC_Point2d>(cam.TrasformPoint(wnd.mouse.GetPos()))))
						i->SetSelectionFlag(false);
					}
					else
					{
						if (i->IsInRange(static_cast<JC_Point2d>(cam.TrasformPoint(wnd.mouse.GetPos()))))
							i->SetSelectionFlag(true);
					}
				}
			}


		}
		break;
	}
	break;
	}

	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{

		wnd.shape = MainWindow::MWShapeState::Null;

		input = 0;

		for (auto &c : circles)
		{
			c.SetSelectionFlag(false);
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_DELETE))
	{
		remove_erase_if(circles, std::mem_fn(&Circle::ReadyForRemoval));

	}

	const float speed = 3.0f;
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		cam.MoveBy({ 0.0f,-speed });
	}
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		cam.MoveBy({ 0.0f,speed });
	}
	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		cam.MoveBy({ -speed,0.0f });
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		cam.MoveBy({ speed,0.0f });
	}

}


void Game::UpdateModel()
{
	for (auto &c : circles)
	{
		c.UpdateColor();
	}
}



void Game::ComposeFrame()
{
	for (auto &c : circles)
	{
		c.Draw(cam);
	}

	//ct.DrawClosedPolyline(Star::Make(200, 75.0,7), Colors::Red);
}

