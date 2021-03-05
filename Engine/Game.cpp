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

#include "Mat3.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	cub(0.5f)
{}

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
	static auto rotZ = Mat3::RotateZ(0.0174533);
	static auto rotY = Mat3::RotateY(0.0174533);
	static auto rotX = Mat3::RotateX(0.0174533);

	auto lineIndexBuffer	= cub.getLineIndexBuffer();
	auto trigIB = cub.getTriangleIndexBuffer();
	auto& vertexBuffer	= cub.getVertexBuffer();

	if (wnd.kbd.KeyIsPressed(0x5A))
	{
		for (auto& i : vertexBuffer)
			i = i * rotZ;
	}
	if (wnd.kbd.KeyIsPressed(0x59))
	{
		for (auto& i : vertexBuffer)
			i = i * rotY;
	}
	if (wnd.kbd.KeyIsPressed(0x58))
	{
		for (auto& i : vertexBuffer)
			i = i * rotX;
	}

	std::vector<Vec3> tempVertexBuffer= cub.getVertexBuffer();

	//do backface culling here on tempVertexBuffer..
	std::vector<bool> cullFlags;
	cullFlags.resize(trigIB.size() / 3, false);

	for (auto& i : tempVertexBuffer)
		i.z = i.z + 2.0f;

	for (int i = 0, trigCounter = 0; i < trigIB.size()/3; i++, trigCounter = trigCounter + 3)
	{
		Vec3 v0 = tempVertexBuffer[trigIB[trigCounter]];
		Vec3 v1 = tempVertexBuffer[trigIB[trigCounter+1]];
		Vec3 v2 = tempVertexBuffer[trigIB[trigCounter+2]];

		auto normal = ((v1 - v0).cross(v2 - v0));

		cullFlags[i] = (normal.dot(v0) > 0.0f);
	}

	

	int triangleCounter = 0;
	for (int i = 0; i < trigIB.size(); i = i + 3)
	{
		if(cullFlags[triangleCounter++] == false)
			gfx.DrawTriangle(pubeToScreenTransformer::getCoordinatesInScreenSpace(tempVertexBuffer[trigIB[i]]),
							 pubeToScreenTransformer::getCoordinatesInScreenSpace(tempVertexBuffer[trigIB[i + 1]]),
							 pubeToScreenTransformer::getCoordinatesInScreenSpace(tempVertexBuffer[trigIB[i + 2]]), colors[triangleCounter]);	
	}
}