#pragma once

/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Star.h																	  *
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

#include <vector>
#include "JC_Vector2.h"
#include "JC_Math.h"

class Star
{
public:
	static std::vector<JC_Point2d> Make(float outerRadius, float innerRadius, int nFlares = 5)
	{
		std::vector<JC_Point2d> star;
		star.reserve(nFlares * 2);
		const double dTheta = 2.0f * PI_D / double(nFlares * 2);
		for (int i = 0; i < nFlares * 2; i++)
		{
			const double rad = (i % 2 == 0) ? outerRadius : innerRadius;
			star.emplace_back(
				rad * cos(double(i) * dTheta),
				rad * sin(double(i) * dTheta)
			);
		}
		return star;
	}
};