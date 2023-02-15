
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	enum SCENE_NUM
	{
		SCENE_1 = 0,
		SCENE_LIGHTSOURCE,
		SCENE_TEXT,
		TOTAL_SCENE
	};

	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

private:

	//Declare a window object
	StopWatch m_timer;
	bool enablePointer = false;
	bool showPointer = true;
	SCENE_NUM sceneNum;
};

#endif