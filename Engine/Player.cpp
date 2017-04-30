#include "Player.h"

Player::Player(const Vec2& playerPostion, float halfWidth, float halfHeight) :
	mPosition(playerPostion),
	mHalfWidth(halfWidth),
	mHalfHeight(halfHeight),
	mVelocity(200.0f, 100.0f),
	mMissile(Vec2((mPosition.x + 1.0f), mPosition.y - 20.0f))
{
}

void Player::WallCollision(const RectF& wall)
{
	const RectF playerRect = GetRect();

	if (playerRect.left < wall.left)
	{
		mPosition.x += wall.left - playerRect.left;
	}
	else if (playerRect.right > wall.right)
	{
		mPosition.x -= playerRect.right - wall.right;
	}

	mMissile.Collision(wall);
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

	if (kbd.KeyIsPressed(VK_SPACE))
	{
		mIsShooting = true;
		mMissile.Update(delta);
	}
}

void Player::DrawPlayer(Graphics& gfx)
{
	//RectF playerRect = GetRect();
	//gfx.DrawRect(playerRect, Colors::White);
	SpriteCodex::DrawPlayer(mPosition, gfx);

	if (mIsShooting)
	{
		mMissile.DrawProjectile(gfx);
	}
}

RectF Player::GetRect() const
{
	return RectF::FromCenter(mPosition, mHalfWidth, mHalfHeight);
}
