#include "Board.h"

Board::Board(int in_width, int in_height)
	:
	width(in_width),
	height(in_height)
{
}

void Board::DrawCell(const Vei2& gridPos, Graphics& gfx, Color c)
{
	gfx.DrawRectDim(gridPos * cellSize, cellSize, cellSize, c);
}