#pragma once

#include "Screen.h"
#include <glfw3.h>
#include <glm\glm.hpp>

class UIManager
{
public:
	UIManager();

	void openScreen();
	void closeScreen();
	void changeScreenTo();

	void mouseMove(GLFWwindow * windowPtr, double x, double y);
	glm::vec2 getCursorPos(GLFWwindow * windowPtr);
	void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods);
	void keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods);

	void update(float dt);

	void draw();

	~UIManager();

private:
	Screen** _screenList;
};

