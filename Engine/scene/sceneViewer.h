#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<memory>

#include "graphics/MainWindow.h"
#include "graphics/Graphics.h"

#include "scene.h"

class Game;

class sceneViewer
{
private:
	sceneViewer(MainWindow* wnd,Graphics* gfx)
		: m_wnd(wnd),m_gfx(gfx)
	{}

public:
	void updateDraw(); //this method is expected to be called every frame..

	template<typename T,typename... values>
	void addScene()	{	
		sceneContainer.emplace_back(std::make_unique<T>(m_wnd,m_gfx,(values)...));	
	}

private:
	MainWindow* m_wnd;
	Graphics* m_gfx;

	unsigned int nextSceneButton = 0x09; //TAB key
	int currentScene = -1;
	std::vector<std::unique_ptr<scene>> sceneContainer;

	friend class Game;
};