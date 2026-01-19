#include "FrameTimer.h"

FrameTimer::FrameTimer()
{
	last = std::chrono::steady_clock::now();
}

float FrameTimer::GetDeltaTime()
{
	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = current - last;
	last = current;
	return deltaTime.count();
}