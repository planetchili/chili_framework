#pragma once
#include <vector>
#include "Vec2.h"
#include "Vec3.h"

class pubeToScreenTransformer
{
public:
	pubeToScreenTransformer() = default;

	static Vec2 getCoordinatesInScreenSpace(const Vec3 pubeCoordinates,const unsigned int screenWidth = 640u,const unsigned int screenHeight = 640u)
	{
		Vec2 screenCoordinates = { -99.0f,-99.0f };

		screenCoordinates.x = pubeCoordinates.x / pubeCoordinates.z;
		screenCoordinates.x = screenCoordinates.x * float(screenWidth / 2);
		screenCoordinates.x = screenCoordinates.x + (screenWidth / 2);

		screenCoordinates.y = pubeCoordinates.y / pubeCoordinates.z;
		screenCoordinates.y = screenCoordinates.y * float(screenHeight / 2);
		screenCoordinates.y = screenCoordinates.y * -1.0f;
		screenCoordinates.y = screenCoordinates.y + (screenHeight / 2);

		return screenCoordinates;
	}
};