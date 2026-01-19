#include "Vei2.h"

Vei2::Vei2(int x, int y)
    :
    x(x),
    y(y)
{
}

Vei2& Vei2::operator+=(const Vei2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

const Vei2 Vei2::operator*(int val) const
{
    return Vei2(x * val, y * val);
}