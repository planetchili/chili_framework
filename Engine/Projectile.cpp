#include "Projectile.h"
#include<assert.h>

Projectile::Projectile(Vec2 projectilePos) :
	mPosition(projectilePos),
	mVelocity(0.0f, 100.0f)
{
}

void Projectile::Update(float deltaTime)
{
	//assert(mIsDestroyed == false);
	mPosition -= mVelocity * deltaTime;
}

bool Projectile::Collision(const RectF& rect)
{
	//assert(mIsDestroyed != false);

	const RectF projectileRect = GetRect();

	if (projectileRect.isOVerlapping(rect))
	{
		mIsDestroyed = true;
		return true;
	}

	return false;
}

void Projectile::DrawProjectile(Graphics & gfx) const
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