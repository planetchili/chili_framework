#pragma once

#include <vector>
#include "Star.h"
#include "random"
#include "FrameTimer.h"

class StarfieldSimulation
{

public:
	StarfieldSimulation(int amount, int _width, int _height)
		:
		width(_width),
		height(_height)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int<> xDist(0, width);
		std::uniform_int<> yDist(0, height);
		//std::uniform_real_distribution<> xDist(0, width);
		//std::uniform_real_distribution<> yDist(0, height);

		for (int i = 0; i < amount; i++)
		{
			stars.emplace_back(xDist(gen), yDist(gen));
		}
	}

	void UpdateFrame()
	{
		for (auto& star : stars)
		{
			star.Update(fTimer.Mark(), width/2 ,height/2);
		}
	}

	void ComposeFrame(Graphics& gfx)
	{
		for (auto& star : stars)
		{
			star.Draw(gfx, width, height);
		}
	}
private:
	FrameTimer fTimer;
	std::vector<Star> stars;
	const int width;
	const int height;
};