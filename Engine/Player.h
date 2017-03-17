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
	Player(const Vec2& playerPostion);
	bool PlayerCollision(const RectF& projectile) const;
	void Update(const Keyboard& kbd, float delta);
	void DrawPlayer(Graphics& gfx);
private:
	Vec2 mPosition;
	Vec2 mVelocity;
	bool mIsShooting;
	static constexpr float speed = 300.0f;
};