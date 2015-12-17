#pragma once
#include "Screen.h"
class ScreenHUD :
	public Screen
{
public:
	ScreenHUD(GLFWwindow * pPointer, GLuint pProgram);

	int init();
	virtual void keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods);

	~ScreenHUD();
};

