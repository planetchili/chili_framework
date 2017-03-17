#include "Player.h"

Player::Player(const Vec2& playerPostion, float halfWidth, float halfHeight) :
	mPosition(playerPostion),
	mHalfWidth(halfWidth),
	mHalfHeight(halfHeight),
	mVelocity(200.0f, 100.0f)
{
}

void Player::WallCollision(const RectF& wall)
{
	const RectF playerRect = GetRect();

	if (mPosition.x < wall.left)
	{
		mPosition.x += wall.left - playerRect.left;
	}
	else if (playerRect.right > wall.right)
	{
		mPosition.x -= playerRect.right - wall.right;
	}
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

void Player::DrawPlayer(Graphics& gfx) const
{
	RectF playerRect = GetRect();
	gfx.DrawRect(playerRect, Colors::White);
	SpriteCodex::DrawPlayer(mPosition, gfx);
}

RectF Player::GetRect() const
{
	return RectF::FromCenter(mPosition, mHalfWidth, mHalfHeight);
}
