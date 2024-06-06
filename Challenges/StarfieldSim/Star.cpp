#include "Star.h"
#include "math.h"
#include "random"
Star::Star(float _x, float _y)
	:
	x(_x),
	y(_y)
{
}

void Star::Update(const float dt, const int centerX, const int centerY)
{
	float dirX = x - centerX;
	float dirY = y - centerY;

	const float mag = sqrtf(dirX * dirX + dirY * dirY);
	dirX /= mag;
	dirY /= mag;

	x += dirX * speed;
	y += dirY * speed;
	
	size = std::max(abs(centerX - (int)x) / (centerX / 10), abs(centerY - (int)y) / (centerY / 10));
}

void Star::Draw(Graphics& gfx, int width, int height)
{
	bool outOfBounds = true;
	for (int Y = (int)y - size; Y <= (int)y + size; Y++)
	{
		for (int X = (int)x - size; X <= (int)x + size; X++)
		{
			if ((size == 0 || sqrtf((float)((X - x) * (X - x) + (Y - y) * (Y - y))) <= size) && Graphics::InBounds(X, Y, 0, width, 0, height))
			{
				gfx.PutPixel(X,Y,Colors::White);
				outOfBounds = false;
			}
		}
	}
	if (outOfBounds)
	{
		Reset(width, height);
	}
}

void Star::Reset( int width, int height)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	int sizeSquare = 50;
	std::uniform_real_distribution<> xDist(width /2 - sizeSquare , width / 2 + sizeSquare);
	std::uniform_real_distribution<> yDist(height / 2 - sizeSquare, height / 2 + sizeSquare);

	x = xDist(gen);
	y = yDist(gen);
}


