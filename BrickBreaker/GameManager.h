#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm\glm.hpp>
#include "GameWorld.h"
#include "UIManager.h"

class GameManager
{
public:
	GameManager(GLFWwindow * pPointer, GLuint pProgram);
	~GameManager();

	void run();
	void load();
	void endLevel();
	void endGame();
	void startGame();
	void setDefaults();

	void mouseMove(GLFWwindow * windowPtr, double x, double y);
	glm::vec2 getCursorPos(GLFWwindow * windowPtr);
	void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods);
	void keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods);

private:
	GLFWwindow * windowPtr;
	GLuint program;

	GameWorld * _world = nullptr;
	Screen * _ui = nullptr;

};

