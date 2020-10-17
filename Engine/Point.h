#pragma once

class Point
{
private:
	int x;
	int y;

public:
	Point(int x, int y) : x(x), y(y) {}
	int GetX() { return x; }
	int GetY() { return y; }
};
