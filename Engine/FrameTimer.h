#pragma once

#include <chrono>

class FrameTimer
{
public:
	FrameTimer();
	float GetDeltaTime();
private:
	std::chrono::steady_clock::time_point last;
};