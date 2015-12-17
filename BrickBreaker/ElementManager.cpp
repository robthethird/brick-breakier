#include "ElementManager.h"
#include <algorithm>

ElementManager::ElementManager()
{
}

//Returns pointer for chaining
GameObject * ElementManager::addElement(GameObject * g)
{
	_elements.push_back(g);
	return g;
}

void ElementManager::removeElement(GameObject * g)
{
	//NOTE this does not delete object
	_elements.erase(std::remove(_elements.begin(), _elements.end(), g), _elements.end());
}

void ElementManager::updateElements(float dt)
{
	int i = _elements.size();
	while (i-- > 0) {
		if (_elements[i]->doDelete) {
			delete _elements[i];
			_elements.erase(_elements.begin() + i);
		}
		else {
			_elements[i]->update(dt);
		}
	}
}

void ElementManager::drawElements()
{
	int i = _elements.size();
	while (i-- > 0) {
		if (_elements[i]->doDelete) {
			delete _elements[i];
			_elements.erase(_elements.begin() + i);
		}
		else {
			_elements[i]->draw();
		}
	}
}

ElementManager::~ElementManager()
{
	int i = _elements.size();
	while (i-- > 0) {
		delete _elements[i];	
	}
}
