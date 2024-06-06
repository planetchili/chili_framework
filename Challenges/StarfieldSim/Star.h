#pragma once

#include "Graphics.h"

class Star
{
public:
	Star(float _x, float _y);
	void Update(const float dt, const int centerX, const int centerY);
	void Draw(Graphics& gfx, int width, int height);
	void Reset( int width, int height);
public:
	const float speed = 0.3f;
	int size = 1;
	float x;
	float y;
};