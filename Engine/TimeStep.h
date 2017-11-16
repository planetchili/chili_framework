#pragma once

#include <chrono>

class TimeStep
{
public:
	TimeStep();
	float Mark();
private:
	std::chrono::steady_clock::time_point last;
};