#pragma once

#include "Rectangle.h"

namespace Fighters
{
	class Square : public Rectangle
	{
	public:
		Square(Point position, Color color, int height);
	};
}
