#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "graphics/MainWindow.h"
#include "graphics/Graphics.h"

class sceneViewer;

class scene
{
public:
	scene() = delete;
	scene(MainWindow* wnd, Graphics* gfx)
		:wnd(wnd), gfx(gfx)
	{}
	virtual ~scene() {};

	virtual void composeFrame() = 0;
	virtual void drawFrame() = 0;

private:
	void drawScene() {
		composeFrame();
		drawFrame();
	}

protected:
	MainWindow* wnd = nullptr;
	Graphics* gfx	= nullptr;

	friend class sceneViewer;
};