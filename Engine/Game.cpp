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
	gfx( wnd )
{
//this example is written in the constructor just because this magic method executes only once and the game is essentially a singleton so the example only executes once per program if placed here.. have fun deleting this example but maybe use them..
	std::string name("empyreani");

	//console logging usuage.. just call the macro and pass tasty arguments and it logs to the screen
	logConsoleTrace("tracing that PutPixel is working... this {} is a variable and another rvalue.. {}", name, 69);
	logConsoleInfo("info that PutPixel is working... this {} is a variable and another rvalue..", name, 255.0f);
	logConsoleWarn("warn that PutPixel is working :>... this {} is a variable and another rvalue.. ", name, 73);
	logConsoleError("error that PutPixel is working :<... this {} is a variable and another rvalue.. ", name, 'p');
	logConsoleCritical("critical that PutPixel is working???... this {} is a variable and another rvalue..", name, 69.0);

	//this macro logs into a file.. to set the folder and file name refer log.h the api as of this writing cannot... the default folder is engine/log with respect to the .sln file you just clicked
	logFileTrace("this is level 0 of the log and this {} is a variable and another rvalue.. {}", name, 69);
	logFileInfo("this is level 1 of the log and this {} is a variable and another rvalue.. {}", name, 255.0);
	logFileWarn("this is level 2 of the log and this {} is a variable and another rvalue.. {}", name, 73);
	logFileError("this is level 3 of the log and this {} is a variable and another rvalue.. {}", name, 'p');
	logFileCritical("this is level 4 of the log and this {} is a variable and another rvalue.. {}", name, 69.0);

	//this macro logs both into file and console.. yay for double vision and a slight nay for performance in debug ONLY..
	logTrace("this is level 0 of the log and this {} is a variable and another rvalue.. {}", name, 69);
	logInfo("this is level 1 of the log and this {} is a variable and another rvalue.. {}", name, 255.0f);
	logWarn("this is level 2 of the log and this {} is a variable and another rvalue.. {}", name, 73);
	logError("this is level 3 of the log and this {} is a variable and another rvalue.. {}", name, 'p');
	logCritical("this is level 4 of the log and this {} is a variable and another rvalue.. {}", name, 69.0);
//end---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
}

void Game::ComposeFrame()
{
	
	if(!wnd.kbd.KeyIsPressed(VK_SPACE))
		gfx.PutPixel(100, 100, Colors::Yellow);
}
