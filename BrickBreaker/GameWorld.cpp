#include "GameWorld.h"

#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glew.h>
#include <glfw3.h>
#include "GameObject.h"
#include "GameData.h"
#include "shaderHelpers.h"
#include "ElementManager.h"
#include "Ball.h"
#include "Console.h"

#include "SoundManager.h"

GameWorld::GameWorld(GLFWwindow *pPointer, GLuint pProgram)
{
	windowPtr = pPointer;
	program = pProgram;

	GameWorld::init();
}

int GameWorld::init() {
	mouseDown = false;

	_camera = new CameraGameObject();
	_camera->setPosition(glm::vec3(0, 16.5f, 16.8875f));
	_camera->setPitch(-0.375f);
	_grid = new Grid(glm::vec3(0, 0, -25), glm::vec3(20, 10, 30), glm::vec3(4, 1, 10));
	
	//paddle
	_paddle = _elementManager.addElement(new GameObject(GameData::instance().getObject("cube")));
	_paddle->setScale(glm::vec3(3.f, 0.75f, 0.35f));
	_grid->add(_paddle->getCollider());

	//arena
	_leftWall = _elementManager.addElement(new GameObject(GameData::instance().getObject("cube")));
	_rightWall = _elementManager.addElement(new GameObject(GameData::instance().getObject("cube")));
	_rearWall = _elementManager.addElement(new GameObject(GameData::instance().getObject("cube")));
	_leftWall->setPosition(glm::vec3(-15.f, 0.f, -25.f));
	_leftWall->setScale(glm::vec3(0.5f, 1.f, 25.f));
	_rightWall->setPosition(glm::vec3(15.f, 0.f, -25.f));
	_rightWall->setScale(glm::vec3(0.5f, 1.f, 25.f));
	_rearWall->setPosition(glm::vec3(0.f, 0.f, -50.f));
	_rearWall->setScale(glm::vec3(15.0f, 1.f, 0.5f));
	_grid->add(_leftWall->getCollider());
	_grid->add(_rightWall->getCollider());
	_grid->add(_rearWall->getCollider());

	GameObject * tObject = _elementManager.addElement(new GameObject(GameData::instance().getObject("skybox")));
	tObject->setTexture("sky");
	tObject->setScale(glm::vec3(1000, 1000, 1000));

	generateLevel();

	Console::Log("Initialized");
	return 0;
}


void GameWorld::update(float dt) {
	
	_camera->update(dt);
	_grid->clean();
	_elementManager.updateElements(dt);
	

	//camera movement
	if (glfwGetKey(windowPtr, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(windowPtr, &xpos, &ypos);
		xpos -= 400;
		ypos -= 300;
		_camera->turn(xpos, ypos);

		_camera->setVelocity(glm::vec3(0, 0, 0));
		if (glfwGetKey(windowPtr, GLFW_KEY_W) == GLFW_PRESS) {
			_camera->setVelocity(_camera->getForward() * 10.0f);
		}
		if (glfwGetKey(windowPtr, GLFW_KEY_A) == GLFW_PRESS) {
			_camera->setVelocity(_camera->getRight() * -10.0f);
		}
		if (glfwGetKey(windowPtr, GLFW_KEY_S) == GLFW_PRESS) {
			_camera->setVelocity(_camera->getForward() * -10.0f);
		}
		if (glfwGetKey(windowPtr, GLFW_KEY_D) == GLFW_PRESS) {
			_camera->setVelocity(_camera->getRight() * 10.0f);
		}
		glfwSetCursorPos(windowPtr, 400, 300);
	}
	else {
		_paddle->setVelocity(glm::vec3(0, 0, 0));
		if (glfwGetKey(windowPtr, GLFW_KEY_A) == GLFW_PRESS || 
			glfwGetKey(windowPtr, GLFW_KEY_LEFT) == GLFW_PRESS) {
			if (!OrientedBoundingBox::SATCollision(_paddle->getCollider(), _leftWall->getCollider()))
				_paddle->setVelocity(glm::vec3(1, 0, 0) *-15.0f);
		}
		if (glfwGetKey(windowPtr, GLFW_KEY_D) == GLFW_PRESS ||
			glfwGetKey(windowPtr, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			if (!OrientedBoundingBox::SATCollision(_paddle->getCollider(), _rightWall->getCollider()))
				_paddle->setVelocity(glm::vec3(1, 0, 0) * 15.0f);
			
		}
	}
	
	

	

	_grid->checkCollisions();
	resolveCollisions();
	_grid->clean();

	//update balls
	int i = _balls.size();
	while (i-- > 0) {
		if (_balls[i]->getPosition().z > 2) {
			SoundManager::PlayClip("death");
			_balls[i]->dispose();
			_balls.erase(_balls.begin() + i);
		}
		else {

		}
	}

	//update bricks
	i = _bricks.size();
	while (i-- > 0) {
		if (_bricks[i]->doDelete) {
			_bricks.erase(_bricks.begin() + i);
		}

	}

	//game over
	if (_bricks.size() == 0) {
		GameData::instance().setBool(GAME_CONSTANT::BOOL_LEVEL_WIN, true);
	}

	if (!_flagLaunch && _balls.size() == 0) {
		_ballCount -= 1;
		_flagLaunch = true;
		if (_ballCount == 0) {
			GameData::instance().setBool(GAME_CONSTANT::BOOL_LEVEL_LOSE, true);
		}
	}
	
}

void GameWorld::resolveCollisions()
{
	std::vector<std::pair<int, int>> collisions = _grid->getCollisionList();
	
	for (int i = 0; i < collisions.size(); i++)
	{
		std::pair<int, int> coll = collisions[i];
		
		int j = _balls.size();
		while (j-- > 0) {
			Ball * tBall = (Ball*)_balls[j];
			if (coll.first == tBall->getCollider()->uid) {
				//resolve
				
				//paddle
				SoundManager::PlayClip("blip");
				if (coll.second == _paddle->getCollider()->uid) {
					glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _paddle->getCollider());
					tBall->setPosition(tBall->getPosition() + tv);
					tBall->collideWithOther(tv);
					tBall->applyForce(_paddle->getVelocity() * -10.f);
				}

				//walls
				if (coll.second == _rearWall->getCollider()->uid) {
					glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _rearWall->getCollider());
					tBall->setPosition(tBall->getPosition() + tv);
					tBall->collideWithOther(glm::vec3(0, 0, 1));
				}
				if (coll.second == _leftWall->getCollider()->uid) {
					glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _leftWall->getCollider());
					tBall->setPosition(tBall->getPosition() + tv);
					tBall->collideWithOther(glm::vec3(-1, 0, 0));
				}
				if (coll.second == _rightWall->getCollider()->uid) {
					glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _rightWall->getCollider());
					tBall->setPosition(tBall->getPosition() + tv);
					tBall->collideWithOther(glm::vec3(1, 0, 0));
				}

				//bricks
				for (int b = 0; b < _bricks.size(); b++) {
					if (coll.second == _bricks[b]->getCollider()->uid) {
						glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _bricks[b]->getCollider());
						tBall->setPosition(tBall->getPosition() + tv);
						tBall->collideWithBrick(tv);
						_bricks[b]->collideWithBall();
						return;
						break;
					}
				}

				//other balls
				int k = j;
				while (k-- > 0) {
					if (coll.second == _balls[k]->getCollider()->uid) {
						glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _balls[k]->getCollider());
						tBall->setPosition(tBall->getPosition() + tv);
						tBall->collideWithNormal(_balls[k]->getPosition() - tBall->getPosition());
						_balls[k]->collideWithNormal(tBall->getPosition() - _balls[k]->getPosition());
						break;
					}
				}

				break;
			}
			if (coll.second == _balls[j]->getCollider()->uid) {
				//resolve
				
				//paddle
				SoundManager::PlayClip("blip");
				if (coll.first == _paddle->getCollider()->uid) {
					glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _paddle->getCollider());
					tBall->setPosition(tBall->getPosition() + tv);
					tBall->collideWithOther(tv);
					tBall->applyForce(_paddle->getVelocity() * -10.f);
				}

				//walls
				if (coll.first == _rearWall->getCollider()->uid) {
					glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _rearWall->getCollider());
					tBall->setPosition(tBall->getPosition() + tv);
					tBall->collideWithOther(glm::vec3(0, 0, 1));
				}
				if (coll.first == _leftWall->getCollider()->uid) {
					glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _leftWall->getCollider());
					tBall->setPosition(tBall->getPosition() + tv);
					tBall->collideWithOther(glm::vec3(-1, 0, 0));
				}
				if (coll.first == _rightWall->getCollider()->uid) {
					glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _rightWall->getCollider());
					tBall->setPosition(tBall->getPosition() + tv);
					tBall->collideWithOther(glm::vec3(1, 0, 0));
				}

				//bricks
				for (int b = 0; b < _bricks.size(); b++) {
					if (coll.first == _bricks[b]->getCollider()->uid) {
						glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _bricks[b]->getCollider());
						tBall->setPosition(tBall->getPosition() + tv);
						tBall->collideWithBrick(tv);
						_bricks[b]->collideWithBall();
						return;
						break;
					}
				}

				//other balls
				int k = j;
				while (k-- > 0) {
					if (coll.first == _balls[k]->getCollider()->uid) {
						glm::vec3 tv = OrientedBoundingBox::MinimumTranslationVector(tBall->getCollider(), _balls[k]->getCollider());
						tBall->setPosition(tBall->getPosition() + tv);
						tBall->collideWithNormal(_balls[k]->getPosition() - tBall->getPosition());
						_balls[k]->collideWithNormal(tBall->getPosition() - _balls[k]->getPosition());
						break;
					}
				}

				break;
			}
		}
	}
}

void GameWorld::draw() {
	// Projection matrix
	//800x600 = 4:3
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	// Camera matrix
	glm::mat4 view = glm::lookAt(
		_camera->getLocation(), //camera
		_camera->getLookAt(), //look at
		_camera->getUp()  // up vector 
		);


	setShaderMatrix(program, "viewMatrix", projection*view);

	
	_elementManager.drawElements();
}

void GameWorld::mouseMove(GLFWwindow* windowPtr, double x, double y) {

}

glm::vec2 GameWorld::getCursorPos(GLFWwindow* windowPtr) {
	double xpos, ypos;
	glfwGetCursorPos(windowPtr, &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(windowPtr, &width, &height);
	glm::vec2 cursorPosition;
	cursorPosition.x = 2.0f * ((float)xpos / (float)width) - 1.0f;
	cursorPosition.y = (2.0f * ((float)ypos / (float)height) - 1.0f) * -1.0f;
	return cursorPosition;
}

void GameWorld::mouseClick(GLFWwindow * windowPtr, int button, int action, int mods) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		switch (action) {
		case GLFW_PRESS:
			mouseDown = true;
			prevMouse = GameWorld::getCursorPos(windowPtr);
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

void GameWorld::keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods) {
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
			if (_flagLaunch) {
				SoundManager::PlayClip("boing");
				_balls.push_back(_elementManager.addElement(new Ball(_paddle->getPosition().x)));
				_grid->add(_balls[_balls.size() - 1]->getCollider());
				_flagLaunch = false;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}


}

void GameWorld::generateLevel()
{
	//ball(s)
	_flagLaunch = true;
	_ballCount = 3;
	

	//bricks
	switch (GameData::instance().getInt(GAME_CONSTANT::INT_LEVEL)) {
	case 0:
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		break;
	case 1:
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -18), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -18), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -18), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -18), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -28), 3)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -28), 3)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -28), 3)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -28), 3)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -38), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -38), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -38), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -38), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -38), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -38), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		break;
	case 2:
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -24), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -22), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -20), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -16), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -16), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -20), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -22), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -24), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -36), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-3.5f, 0, -34), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-5.5f, 0, -32), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-3.5f, 0, -30), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(0.f, 0, -32), 3)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(3.5f, 0, -30), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(5.5f, 0, -32), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(3.5f, 0, -34), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -36), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -40), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -42), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -44), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -46), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -46), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -44), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -42), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -40), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		break;
	default:
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -24), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -22), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -20), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -16), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -16), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -18), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -20), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -22), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -24), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -36), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-3.5f, 0, -34), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-5.5f, 0, -32), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-3.5f, 0, -30), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(0.f, 0, -32), 3)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -28), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(3.5f, 0, -30), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(5.5f, 0, -32), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(3.5f, 0, -34), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -36), 2)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());

		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-13.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-10.5f, 0, -40), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-7.5f, 0, -42), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-4.5f, 0, -44), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(-1.5f, 0, -46), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(1.5f, 0, -46), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(4.5f, 0, -44), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(7.5f, 0, -42), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(10.5f, 0, -40), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		_bricks.push_back((Brick *)_elementManager.addElement(new Brick(glm::vec3(13.5f, 0, -38), 1)));
		_grid->add(_bricks[_bricks.size() - 1]->getCollider());
		break;
	}
	
}



GameWorld::~GameWorld()
{
	delete _camera;
	delete _grid;
}
