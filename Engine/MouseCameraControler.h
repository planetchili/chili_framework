#pragma once
#include "Camera.h"
#include "Mouse.h"

class MouseCameraController
{
public:
	MouseCameraController(Mouse& mouse, Camera& cam)
		:
		mouse(mouse),
		cam(cam)
	{}
	void UpdateInput()
	{
		while (!mouse.IsEmpty())
		{
			const auto e = mouse.Read();
			switch (e.GetType())
			{
			case Mouse::Event::Type::WheelPress:
				engaged = true;
				lastPos = e.GetPos();
				break;
			case Mouse::Event::Type::WheelRelease:
				engaged = false;
				break;
			case Mouse::Event::Type::WheelUp:
				cam.SetScale(cam.GetScale() * zoomFactor);
				break;
			case Mouse::Event::Type::WheelDown:
				cam.SetScale(cam.GetScale() / zoomFactor);
				break;
			}
		}

		if (engaged)
		{
			const auto curPos = (JC_Point2d)mouse.GetPos();
			auto delta = curPos - lastPos;
			delta.x = -delta.x; // fixes the disconnect between screen coords and math coords
			cam.MoveBy(delta / cam.GetScale());
			lastPos = curPos;
		}
	}
private:
	static constexpr float zoomFactor = 1.05f;
	bool engaged = false;
	JC_Point2d lastPos;
	Mouse& mouse;
	Camera& cam;
};