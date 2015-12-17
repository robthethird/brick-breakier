#pragma once

#include <glew.h>
#include <glfw3.h>
#include "GameObject.h"
#include "CameraGameObject.h"
#include "ElementManager.h"
#include "Grid.h"
#include <vector>

#include "Brick.h"

class GameWorld
{
public:
	GameWorld(GLFWwindow * pPointer, GLuint pProgram);

	int init();
	
	void generateLevel();
	void update(float dt);
	void draw();

	void mouseMove(GLFWwindow * windowPtr, double x, double y);
	glm::vec2 getCursorPos(GLFWwindow * windowPtr);
	void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods);
	void keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods);

	~GameWorld();

private:
	GLFWwindow * windowPtr;
	GLuint program;

	ElementManager _elementManager;
	bool mouseDown;
	glm::vec2 prevMouse;

	CameraGameObject * _camera;

	Grid * _grid;
	void resolveCollisions();

	//Game specific
	GameObject * _paddle;
	GameObject * _leftWall;
	GameObject * _rightWall;
	GameObject * _rearWall;
	std::vector<GameObject *> _balls;
	std::vector<Brick *> _bricks;

	int _ballCount;
	bool _flagLaunch;
};

