#pragma once
#include "JC_Vector2.h"
#include "Camera.h"

class Shape

{
public:
	bool ReadyForRemoval() const;

	void virtual Draw(Camera cam) = 0;
	bool virtual IsInRange(JC_Point2d mousein) = 0;

	void UpdateColor();
	void SetSelectionFlag(bool flag);

protected:

	Shape(JC_Point2d pos_in, Color& c)
		:
		Centre(pos_in),
		C(c)
	{}

	JC_Point2d Centre;
	Color C;
	static constexpr double halfwidth = 10.0;

private:
	bool selectedflag = false;

};