#pragma once
#include <glew.h>
#include <glfw3.h>
#include "GameObject.h"
#include "CameraGameObject.h"
#include "ElementManager.h"

class Screen
{
public:
	Screen(GLFWwindow * pPointer, GLuint pProgram);

	virtual int init();

	virtual void update(float dt);
	void draw();

	virtual void mouseMove(GLFWwindow * windowPtr, double x, double y);
	virtual glm::vec2 getCursorPos(GLFWwindow * windowPtr);
	virtual void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods);
	virtual void keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods);

	virtual ~Screen();

protected:
	GLFWwindow * windowPtr;
	GLuint program;

	ElementManager _elementManager;
	bool mouseDown;
	glm::vec2 prevMouse;

	CameraGameObject * _camera;
};

