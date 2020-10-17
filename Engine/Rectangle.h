#pragma once

#include "Fighter.h"
#include "consts.h"

namespace Fighters
{
	class Rectangle : public Fighter
	{
	private:
		int width; // the position field is top left of the shape
		int height;

	public:
		Rectangle(Point position, Color color, int width, int height, int life_points = RECTANGLE_LIFE_POINTS, int damage = RECTANGLE_DAMAGE);
		void draw(Graphics& gfx);

	};
}
