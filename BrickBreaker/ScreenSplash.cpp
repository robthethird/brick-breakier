#include "ScreenSplash.h"
#include "GameData.h"
#include "Console.h"
#include "SoundManager.h"

ScreenSplash::ScreenSplash(GLFWwindow * pPointer, GLuint pProgram) : Screen(pPointer, pProgram)
{
	ScreenSplash::init();
}

int ScreenSplash::init() {

	GameObject * tObject = _elementManager.addElement(new GameObject(GameData::instance().getObject("ui_quad")));
	tObject->setPosition(glm::vec3(400, 300, 1));
	tObject->setScale(glm::vec3(400, 300, 1));
	tObject->setTexture("title");

	return 0;
}


void ScreenSplash::keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_Q:
		switch (action) {
		case GLFW_PRESS:
			glfwSetWindowShouldClose(windowPtr, true);
			break;
		default:
			break;
		}
		break;
	case GLFW_KEY_SPACE:
		switch (action) {
		case GLFW_PRESS:
			GameData::instance().setBool(GAME_CONSTANT::BOOL_GAME_START, true);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}


}

ScreenSplash::~ScreenSplash()
{
}
