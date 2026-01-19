#pragma once

#include "Vei2.h"
#include "Board.h"
#include "Colors.h"

class Player
{
public:
	Player(const Vei2& in_pos);
	void Draw(Board& brd, Graphics& gfx, Color c) const;
	void MoveBy(const Vei2& delta_loc);
private:
	Vei2 pos;
	Color c;
};