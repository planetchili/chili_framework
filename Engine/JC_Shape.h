#pragma once
#include "JC_Vector2.h"
#include "Camera.h"
class JC_Shape

{
public:


	void virtual Draw(Camera cam_in) = 0;
	bool virtual IsInRange(const JC_Point2d& mouse_in) = 0;
	std::wstring virtual MakeDescription() = 0;

	void UpdateColor()
	{
		if (selectedflag)
			Base_Color = Colors::Yellow;
		else
			Base_Color = Colors::White;
	}

	void SetSelectionFlag(bool flag)
	{
		selectedflag = flag;
	}


	bool ReadyForRemoval() const
	{
		return selectedflag;
	}

	bool IsSelected() const
	{
		return selectedflag;
	}

protected:

	JC_Shape( Color& color_in)
		:
		Base_Color(color_in)
	{}

	Color Base_Color;
	static constexpr double halfwidth = 10.0;

private:
	bool selectedflag = false;

};
