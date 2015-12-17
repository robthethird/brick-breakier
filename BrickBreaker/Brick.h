#pragma once
#include "GameObject.h"

class Brick :
	public GameObject
{
public:
	Brick(glm::vec3 pos, int h);
	~Brick();

	void collideWithBall();

private:
	int hits;
};

