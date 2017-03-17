#include "Player.h"

Player::Player(const Vec2 & playerPostion) :
	mPosition(playerPostion),
	mVelocity(200.0f, 100.0f)
{
}

void Player::Update(const Keyboard& kbd, float delta)
{
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		mPosition.x -= mVelocity.x * delta;
	}
	else if (kbd.KeyIsPressed(VK_RIGHT))
	{
		mPosition.x += mVelocity.x * delta;
	}
}

void Player::DrawPlayer(Graphics& gfx)
{
	SpriteCodex::DrawPlayer(mPosition, gfx);
}
