#include <iostream>
#include "GameWorld.h"
#include "GameManager.h"
#include "shaderHelpers.h"
#include <random>
#include <time.h>


GameManager* game;

void keyPress(GLFWwindow* windowPtr, int key, int scancode, int action, int mods)
{
	//pass through to game
	game->keyPress(windowPtr, key, scancode, action, mods);
}

void mouseMove(GLFWwindow* windowPtr, double x, double y) {
	//pass through to game
	game->mouseMove(windowPtr, x, y);
}

void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods) {
	//pass through to game

	game->mouseClick(windowPtr, button, action, mods);
}

int main(int argc, char ** argv) {
	if (!glfwInit())
		return -1;

	GLFWwindow * windowPtr = glfwCreateWindow(800, 600, "Brick Breakier", NULL, NULL);
	if (!windowPtr) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(windowPtr);
	if (glewInit() != GLEW_OK) return false;

	GLuint program = loadShaderProgram("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
	if (program == 0) {
		
		///something went wrong
		return -1;
	}
	glUseProgram(program);
	glClearColor(0.3f, 0.5f, 0.9f, 1.0f);

	srand((unsigned int)time(0));

	glfwSetMouseButtonCallback(windowPtr, mouseClick);
	glfwSetCursorPosCallback(windowPtr, mouseMove);
	glfwSetKeyCallback(windowPtr, keyPress);

	game = new GameManager(windowPtr, program);
	//game->load();
	game->run();

	return 0;
}