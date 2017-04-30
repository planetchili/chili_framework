#pragma once
#include "Vec2.h"
#include "RectF.h"
#include "Keyboard.h"
#include "FrameTimer.h"
#include "SpriteCodex.h"
#include "Graphics.h"
#include "Projectile.h"

class Player
{
public:
	Player() = default;
	Player(const Vec2& playerPostion, float halfWidth, float halfHeight);
	void WallCollision(const RectF& wall);
	void Update(const Keyboard& kbd, float delta);
	void DrawPlayer(Graphics& gfx);
	RectF GetRect() const;
private:
	Vec2 mPosition;
	float mHalfWidth;
	float mHalfHeight;
	Vec2 mVelocity;
	Projectile mMissile;
	bool mIsShooting = false;
	static constexpr float mSpeed = 300.0f;
};