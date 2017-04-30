#include "Projectile.h"
#include<assert.h>

Projectile::Projectile(Vec2 projectilePos) :
	mPosition(projectilePos),
	mVelocity(0.0f, 400.0f)
{
}

void Projectile::Update(float deltaTime)
{
	mPosition -= mVelocity * deltaTime;
}

bool Projectile::Collision(const RectF& rect)
{
	const RectF projectileRect = GetRect();

	if (!projectileRect.isOVerlapping(rect))
	{
		mIsDestroyed = true;
		return true;
	}

	return false;
}

void Projectile::DrawProjectile(Graphics& gfx) const
{
	RectF projectileRect = GetRect();

	if (!mIsDestroyed)
	{
		gfx.DrawRect(projectileRect, Colors::White);
	}
}

RectF Projectile::GetRect() const
{
	return RectF::FromCenter(mPosition, mHalfWidth, mHalfHeight);
}

void Projectile::SetPosition(const Vec2& pos)
{
	this->mPosition = pos;
}
