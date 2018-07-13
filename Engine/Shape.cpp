#include "Shape.h"

void Shape::UpdateColor()
{
	if (selectedflag)
		C = Colors::Yellow;
	else
		C = Colors::White;
}

void Shape::SetSelectionFlag(bool flag)
{
	selectedflag = flag;
}

bool Shape::ReadyForRemoval() const
{
	return selectedflag;
}