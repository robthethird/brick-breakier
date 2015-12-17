#pragma once
#include "OrientedBoundingBox.h"
#include <vector>
#include <utility>

class Grid
{
public:
	Grid(glm::vec3 c, glm::vec3 h, glm::vec3 r);
	~Grid();

	void print();
	void add(OrientedBoundingBox * b);
	void checkCollisions();
	void clean();
	std::vector<std::pair<int, int>> getCollisionList();

private:
	glm::vec3 center;
	glm::vec3 halfWidths;
	glm::vec3 resolution;

	std::vector<std::pair<int, int>> collisions;

	int convertIndex(int i, int j, int k);
	OrientedBoundingBox getOBB(int i, int j, int k);

	std::vector<std::vector<OrientedBoundingBox*>> * bins;
};

