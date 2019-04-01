/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Mouse.h																				  *
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
#include <queue>
#include "JC_Vector2.h"

class Mouse
{
	friend class MainWindow;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelPress,
			WheelRelease,
			WheelUp,
			WheelDown,
			Move,
			Invalid
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		bool wheelIsPressed;
		int x;
		int y;
	public:
		Event()
			:
			type( Type::Invalid ),
			leftIsPressed( false ),
			rightIsPressed( false ),
			wheelIsPressed( false ),
			x( 0 ),
			y( 0 )
		{}
		Event( Type type,const Mouse& parent )
			:
			type( type ),
			leftIsPressed( parent.leftIsPressed ),
			rightIsPressed( parent.rightIsPressed),
			wheelIsPressed( parent.wheelIsPressed),
			x( parent.x ),
			y( parent.y )
		{}
		bool IsValid() const
		{
			return type != Type::Invalid;
		}
		Type GetType() const
		{
			return type;
		}
		JC_Point2i GetPos() const
		{
			return{ x,y };
		}
		int GetPosX() const
		{
			return x;
		}
		int GetPosY() const
		{
			return y;
		}
		bool LeftIsPressed() const
		{
			return leftIsPressed;
		}
		bool RightIsPressed() const
		{
			return rightIsPressed;
		}
		bool WheelIsPressed() const
		{
			return wheelIsPressed;
		}
	};
public:
	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;
	JC_Point2i GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
	bool WheelIsPressed() const;
	bool IsInWindow() const;
	Mouse::Event Read();
	bool IsEmpty() const
	{
		return buffer.empty();
	}
	void Flush();
private:
	void OnMouseMove( int x,int y );
	void OnMouseLeave();
	void OnMouseEnter();
	void OnLeftPressed( int x,int y );
	void OnLeftReleased( int x,int y );
	void OnRightPressed( int x,int y );
	void OnRightReleased( int x,int y );
	void OnWheelPressed(int x, int y);
	void OnWheelReleased(int x, int y);
	void OnWheelUp( int x,int y );
	void OnWheelDown( int x,int y );
	void TrimBuffer();
private:
	static constexpr unsigned int bufferSize = 4u;
	int x;
	int y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool wheelIsPressed = false;
	bool isInWindow = false;
	std::queue<Event> buffer;
};