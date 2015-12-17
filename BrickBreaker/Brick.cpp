#include "Brick.h"
#include "GameData.h"
#include "Console.h"
#include "SoundManager.h"

Brick::Brick(glm::vec3 pos, int h = 1) : GameObject(GameData::instance().getObject("cube"))
{
	position = pos;
	scale = glm::vec3(1.4f, 0.8, 0.6f);
	hits = h;
	switch (h)
	{
	case 1:
		setTexture("brick1");
		break;
	case 2:
		setTexture("brick2");
		break;
	case 3:
		setTexture("brick3");
		break;
	default:
		setTexture("brick1");
	}
	//shape->setTexture("metal");
}

void Brick::collideWithBall()
{
	//Console::Log("Collide with Ball");
	hits -= 1;
	switch (hits)
	{
	case 1:
		setTexture("brick1");
		break;
	case 2:
		setTexture("brick2");
		break;
	case 3:
		setTexture("brick3");
		break;
	default:
		setTexture("brick1");
	}
	if (hits == 0) {
		SoundManager::PlayClip("break");
		dispose();
	}
}


Brick::~Brick()
{
	GameData::instance().modifyInt(GAME_CONSTANT::INT_SCORE, 100);
}
