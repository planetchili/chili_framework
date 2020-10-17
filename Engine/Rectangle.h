#pragma once

#include "Fighter.h"
#include "consts.h"

class Rectangle : public Fighter
{
private:
	int width; // the position field is top left of the shape
	int height;

public:
	Rectangle(Point position, Color color, int width, int height, int life_points, int damage);
	void draw(Graphics& gfx);

};
