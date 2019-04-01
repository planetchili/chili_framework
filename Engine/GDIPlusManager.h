/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	GDIPlusManager.h																	  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
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
#include "ChiliWin.h"

class GDIPlusManager
{
public:
	// when a gdipman is ctored when no others exist, it will init gdip
	GDIPlusManager();
	// when a gdipman is dtored and it is the last existing, it will cleanup gdip
	~GDIPlusManager();
private:
	// gdip creation token (for API calls)
	static ULONG_PTR token;
	// refcount to keep track of when necessary to init/cleanup
	static int refCount;
};