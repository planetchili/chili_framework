#include "Vei2.h"
#include <cmath>

Vei2::Vei2(int x_in, int y_in)
	:
	x(x_in),
	y(y_in)
{
}

Vei2 Vei2::operator+(const Vei2& rhs) const
{
	return Vei2(x + rhs.x, y + rhs.y);
}

Vei2& Vei2::operator+=(const Vei2& rhs)
{
	return *this = *this + rhs;
}

Vei2 Vei2::operator*(int rhs) const
{
	return Vei2(x * rhs, y * rhs);
}

Vei2& Vei2::operator*=(int rhs)
{
	return *this = *this * rhs;
}

Vei2 Vei2::operator-(const Vei2& rhs) const
{
	return Vei2(x - rhs.x, y - rhs.y);
}

Vei2& Vei2::operator-=(const Vei2& rhs)
{
	return *this = *this - rhs;
}

Vei2 Vei2::operator/(int rhs) const
{
	return Vei2(x / rhs, y / rhs);
}

Vei2& Vei2::operator/=(int rhs)
{
	return *this = *this / rhs;
}

float Vei2::GetLength() const
{
	return std::sqrt(float(GetLengthSq()));
}

int Vei2::GetLengthSq() const
{
	return x * x + y * y;
}