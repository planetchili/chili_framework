/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	ChiliMath.h																			  *
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
#include<algorithm>
#include<utility>
#include <math.h>

constexpr float PI = 3.14159265f;

template <typename T>
inline auto sq( const T& x )
{
	return x * x;
}

template<bool enabler = true>
float normalize(float val, float min, float max)
{
    // Shift to positive to avoid issues when crossing the 0 line
    if (min < 0) {
        max += 0 - min;
        val += 0 - min;
        min = 0;
    }
    // Shift values from 0 - max
    val = val - min;
    max = max - min;
    return std::max(0.0f, std::min(1.0f, val / max));
}

template<typename T>
T interpolateTo(T x, T y, float alpha)
{
    return x + alpha * (y - x);
}