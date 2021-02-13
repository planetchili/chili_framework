/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Mat2.h																				  *
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

#include "Vec2.h"

template <typename T>
class _Mat2
{
public:
	_Mat2& operator=( const _Mat2& rhs )
	{
		memcpy( elements,rhs.elements,sizeof( elements ) );
		return *this;
	}
	_Mat2& operator*=( T rhs )
	{
		for( auto& row : elements )
		{
			for( T& e : row )
			{
				e *= rhs;
			}
		}
		return *this;
	}
	_Mat2 operator*( T rhs ) const
	{
		_Mat2 result = *this;
		return result *= rhs;
	}
	_Mat2 operator*( const _Mat2& rhs ) const
	{
		_Mat2 result;
		for( size_t j = 0; j < 2; j++ )
		{
			for( size_t k = 0; k < 2; k++ )
			{
				T sum = (T)0.0;
				for( size_t i = 0; i < 2; i++ )
				{
					sum += elements[j][i] * rhs.elements[i][k];
				}
				result.elements[j][k] = sum;
			}
		}
		return result;
	}
	static _Mat2 Identity()
	{
		_Mat2 i = { (T)1.0,(T)0.0,(T)0.0,(T)1.0 };
		return i;
	}
	static _Mat2 Rotation( T theta )
	{
		const T cosTheta = cos( theta );
		const T sinTheta = sin( theta );
		_Mat2 r = { 
			cosTheta,	sinTheta,
			-sinTheta,	cosTheta };
		return r;
	}
	static _Mat2 Scaling( T factor )
	{
		_Mat2 s = { factor,(T)0.0,(T)0.0,factor };
		return s;
	}
public:
	// [ row ][ col ]
	T elements[2][2];
};

template<typename T>
_Vec2<T>& operator*=( _Vec2<T>& lhs,const _Mat2<T>& rhs )
{
	return lhs = lhs * rhs;
}

template<typename T>
_Vec2<T> operator*( const _Vec2<T>& lhs,const _Mat2<T>& rhs )
{
	return { 
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1]
	};
}

typedef _Mat2<float> Mat2;
typedef _Mat2<double> Mad2;