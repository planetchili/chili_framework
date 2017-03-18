#pragma once
#include "Vec2.h"
#include "RectF.h"
#include "Graphics.h"

class Projectile
{
public:
	Projectile() = default;
	Projectile(Vec2 projectilePos);
	void Update(float deltaTime);
	bool Collision(const RectF& rect);
	void DrawProjectile(Graphics& gfx) const;
	RectF GetRect() const;
private:
	Vec2 mPosition;
	Vec2 mVelocity;
	static constexpr float mHalfWidth = 1.5f;
	static constexpr float mHalfHeight = 5.0f;
	static constexpr float mSpeed = 5.0f;
	bool mIsDestroyed = false;
};