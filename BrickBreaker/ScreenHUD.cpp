#include "ScreenHUD.h"
#include "GameData.h"

ScreenHUD::ScreenHUD(GLFWwindow * pPointer, GLuint pProgram) : Screen(pPointer, pProgram)
{
	ScreenHUD::init();
}

int ScreenHUD::init() {
	//Screen::init();

	//UI goes here
	GameObject * tObject = _elementManager.addElement(new GameObject(GameData::instance().getObject("ui_quad")));
	tObject->setPosition(glm::vec3(400, 25, 1));
	tObject->setScale(glm::vec3(400, 25, 1));
	tObject->setTexture("ui");

	return 0;
}


void ScreenHUD::keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_Q:
		switch (action) {
		case GLFW_PRESS:
			GameData::instance().setBool(GAME_CONSTANT::BOOL_GAME_OVER, true);
			break;
		default:
			break;
		}
		break;
	case GLFW_KEY_SPACE:
		switch (action) {
		case GLFW_PRESS:
			
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}


}


ScreenHUD::~ScreenHUD()
{
}
