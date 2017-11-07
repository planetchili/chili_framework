#include "TimeStep.h"

using namespace std::chrono;

TimeStep::TimeStep()
{
	last = steady_clock::now();
}

float TimeStep::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

