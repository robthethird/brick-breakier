#pragma once
#include "Screen.h"
class ScreenSplash :
	public Screen
{
public:
	ScreenSplash(GLFWwindow * pPointer, GLuint pProgram);

	int init();

	virtual void keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods);

	~ScreenSplash();
};

