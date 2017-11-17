#pragma once

#include <random>

class Random
{
public:
	Random();
	int NextInt( int min,int max );
private:
	std::mt19937 rng;
};