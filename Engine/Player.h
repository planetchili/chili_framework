#pragma once
#include "Vec2.h"
#include "RectF.h"
#include "Keyboard.h"
#include "FrameTimer.h"
#include "SpriteCodex.h"
#include "Graphics.h"

class Player
{
public:
	Player() = default;
	Player(const Vec2& playerPostion, float halfWidth, float halfHeight);
	bool ProjectileCollision(const RectF& projectile) const;
	void WallCollision(const RectF& wall);
	void Update(const Keyboard& kbd, float delta);
	void DrawPlayer(Graphics& gfx) const;
	RectF GetRect() const;
private:
	Vec2 mPosition;
	float mHalfWidth;
	float mHalfHeight;
	Vec2 mVelocity;
	bool mIsShooting;
	static constexpr float mSpeed = 300.0f;
};