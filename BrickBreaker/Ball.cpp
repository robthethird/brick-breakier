#include "Ball.h"
#include "GameData.h"
#include "mathHelpers.h"

Ball::Ball(float posx) : GameObject(GameData::instance().getObject("sphere"))
{
	scale = glm::vec3(0.7, 0.7, 0.7);
	float angle = randFloat(-pi / 4, pi / 4);
	glm::mat4 rotation = glm::rotate(angle, glm::vec3(0,1,0));
	velocity = glm::vec3(glm::vec4(0, 0, -25,0)*rotation);
	position = glm::vec3(posx, 0, -3);

	combo = 1;

	setTexture("metal");
}

void Ball::collideWithBrick(glm::vec3 n)
{
	collideWithNormal(n);
	GameData::instance().modifyInt(GAME_CONSTANT::INT_SCORE, 10*combo);
	combo++;
}

void Ball::collideWithOther(glm::vec3 n)
{
	combo = 1;
	collideWithNormal(n);
}


Ball::~Ball()
{
}
