#include "Player.h"

Player::Player(const Vei2& in_pos)
{
	pos = in_pos;
}

void Player::Draw(Board& brd, Graphics& gfx, Color c) const
{
	brd.DrawCell(pos, gfx, c);
}

void Player::MoveBy(const Vei2& delta_loc)
{
	pos += delta_loc;
}