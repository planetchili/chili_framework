#pragma once

#include "Graphics.h"
#include "Vei2.h"
#include "Colors.h"

class Board
{
public:
	Board(int in_width, int in_height);
	void DrawCell(const Vei2& gridPos, Graphics& gfx, Color c);
private:
	Color c;
	static constexpr int cellSize = 15;
	int width;
	int height;
};