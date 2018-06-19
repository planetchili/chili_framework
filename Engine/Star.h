#pragma once


#include <vector>
#include "JC_Vector2.h"
#include "JC_Math.h"

class Star
{
public:
	static std::vector<JC_Point2d> Make(float outerRadius, float innerRadius, int nFlares = 5)
	{
		std::vector<JC_Point2d> star;
		star.reserve(nFlares * 2);
		const double dTheta = 2.0f * PI_D / double(nFlares * 2);
		for (int i = 0; i < nFlares * 2; i++)
		{
			const double rad = (i % 2 == 0) ? outerRadius : innerRadius;
			star.emplace_back(
				rad * cos(double(i) * dTheta),
				rad * sin(double(i) * dTheta)
			);
		}
		return star;
	}
};