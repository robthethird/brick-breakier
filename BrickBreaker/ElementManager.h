#pragma once
#include <vector>
#include "GameObject.h"

class ElementManager
{
private:
	std::vector<GameObject*> _elements;
public:
	ElementManager();

	GameObject * addElement(GameObject *);
	void removeElement(GameObject *);
	void updateElements(float dt);
	void drawElements();

	~ElementManager();
};

