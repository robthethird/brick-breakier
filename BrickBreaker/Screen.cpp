#include "Screen.h"
#include "shaderHelpers.h"
#include "GameData.h"
#include "Console.h"

Screen::Screen(GLFWwindow * pPointer, GLuint pProgram)
{
	windowPtr = pPointer;
	program = pProgram;

	Screen::init();
}

int Screen::init() {
	mouseDown = false;

	_camera = new CameraGameObject();
	_camera->setPosition(glm::vec3(0, 0, 5));
	

	return 0;
}

void Screen::update(float dt) {
	
	_camera->update(dt);
	_elementManager.updateElements(dt);

}

void Screen::draw() {
	// Projection matrix
	//800x600 = 4:3
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 view = glm::lookAt(
		_camera->getLocation(), //camera
		_camera->getLookAt(), //look at
		_camera->getUp()  // up vector 
		);


	setShaderMatrix(program, "viewMatrix", projection*view);


	_elementManager.drawElements();
}

void Screen::mouseMove(GLFWwindow* windowPtr, double x, double y) {

}

glm::vec2 Screen::getCursorPos(GLFWwindow* windowPtr) {
	double xpos, ypos;
	glfwGetCursorPos(windowPtr, &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(windowPtr, &width, &height);
	glm::vec2 cursorPosition;
	cursorPosition.x = 2.0f * ((float)xpos / (float)width) - 1.0f;
	cursorPosition.y = (2.0f * ((float)ypos / (float)height) - 1.0f) * -1.0f;
	return cursorPosition;
}

void Screen::mouseClick(GLFWwindow * windowPtr, int button, int action, int mods) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		switch (action) {
		case GLFW_PRESS:
			mouseDown = true;
			break;
		case GLFW_RELEASE:
			mouseDown = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}


}

void Screen::keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_Q:
		switch (action) {
		case GLFW_PRESS:
			
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


Screen::~Screen()
{
	delete _camera;
}
