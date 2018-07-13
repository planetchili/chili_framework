#include "JC_Shape.h"

void JC_Shape::UpdateColor()
{
	if (selectedflag)
		C = Colors::Yellow;
	else
		C = Colors::White;
}

void JC_Shape::SetSelectionFlag(bool flag)
{
	selectedflag = flag;
}

bool JC_Shape::ReadyForRemoval() const
{
	return selectedflag;
}