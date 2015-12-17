#pragma once
#include "GameObject.h"

class Ball :
	public GameObject
{
private:
	int combo;

public:
	Ball(float posx);

	void collideWithBrick(glm::vec3 n);
	void collideWithOther(glm::vec3 n);

	~Ball();
};

