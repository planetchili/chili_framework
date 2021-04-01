#include "sceneViewer.h"

void sceneViewer::updateDraw()
{
	static bool pressed = false;

	//updating part
	if (m_wnd->kbd.KeyIsPressed(nextSceneButton))
	{
		if (pressed == false)
		{
			if (sceneContainer.size() > currentScene + 1)
				currentScene++;
			else if (currentScene != -1)
				currentScene = 0;
		}
	}
	else pressed = false;

	//drawing part
	if (currentScene != -1)
		sceneContainer[currentScene]->drawScene();
}