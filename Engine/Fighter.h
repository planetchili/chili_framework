#pragma once

#include "Point.h"
#include "Colors.h"
#include "Graphics.h"

enum Side {left, right};

class Fighter
{
protected:
	int life_points;
	int damage;
	Point position;
	Color color;
	
public:
	Fighter(int life_points = 0, int damage = 0, Point pos = Point(0,0) , Color color = Color(0,0,0)) : life_points(life_points), damage(damage) , position(pos) , color(color) {}
	virtual void draw(Graphics& gfx) = 0;
	void draw_life_points(Graphics& gfx, Side side);
	void damaged(int damaged_points) { this->life_points -= damaged_points; }
	bool is_dead() { return life_points <= 0; }
	
 };
