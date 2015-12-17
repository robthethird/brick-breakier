#include "Grid.h"
#include "Console.h"
#include <iostream>

Grid::Grid(glm::vec3 c, glm::vec3 h, glm::vec3 r)
{
	center = c;
	halfWidths = h;
	resolution = r;
	bins = new std::vector<std::vector<OrientedBoundingBox*>>(r[0]*r[1]*r[2], std::vector<OrientedBoundingBox*>());
}

OrientedBoundingBox Grid::getOBB(int i, int j, int k)
{
	glm::vec3 es = glm::vec3(halfWidths.x / resolution.x, halfWidths.y / resolution.y, halfWidths.z / resolution.z);
	float cx = center.x + ((float)i - resolution.x / 2)*(halfWidths.x / (resolution.x / 2));
	float cy = center.y + ((float)j - resolution.y / 2)*(halfWidths.y / (resolution.y / 2));
	float cz = center.z + ((float)k - resolution.z / 2)*(halfWidths.z / (resolution.z / 2));
	cx += es.x;
	cy += es.y;
	cz += es.z;
	return OrientedBoundingBox(glm::vec3(cx, cy, cz), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), es);
}

void Grid::add(OrientedBoundingBox * b)
{
	for (int i = 0; i < resolution.x; i++) {
		for (int j = 0; j < resolution.y; j++) {
			for (int k = 0; k < resolution.z; k++) {
				OrientedBoundingBox g = getOBB(i, j, k);
				
				if (OrientedBoundingBox::SATCollision(b, &g)) {
					bins->at(convertIndex(i, j, k)).push_back(b);
					//std::cout << "Adding OBB " << b->uid << " to bin " << convertIndex(i, j, k) << std::endl;
				}
			}
		}
	}
	b->moved = false;
}

//remove any bbs marked for deletion
void Grid::clean()
{
	for (int i = 0; i < resolution.x; i++) {
		for (int j = 0; j < resolution.y; j++) {
			for (int k = 0; k < resolution.z; k++) {

				int m = bins->at(convertIndex(i, j, k)).size();
				while (m-- > 0) {
					OrientedBoundingBox * b = bins->at(convertIndex(i, j, k))[m];

					if (b->doDelete) {
						bins->at(convertIndex(i, j, k)).erase(bins->at(convertIndex(i, j, k)).begin() + m);
					}

				}
			}
		}
	}
}

void Grid::checkCollisions()
{
	
	//check for objects that moved
	std::vector<OrientedBoundingBox *> moved;
	for (int i = 0; i < resolution.x; i++) {
		for (int j = 0; j < resolution.y; j++) {
			for (int k = 0; k < resolution.z; k++) {
				
				int m = bins->at(convertIndex(i, j, k)).size();
				while(m-- > 0) {
					OrientedBoundingBox * b = bins->at(convertIndex(i, j, k))[m];
					
					if (b->doDelete) {
						bins->at(convertIndex(i, j, k)).erase(bins->at(convertIndex(i, j, k)).begin() + m);
					} else if (b->moved) {
						bins->at(convertIndex(i, j, k)).erase(bins->at(convertIndex(i, j, k)).begin() + m);
						//std::cout << "Removing OBB " << b->uid << " from bin " << convertIndex(i, j, k) << std::endl;
						bool exists = false;
						for (int n = 0; n < moved.size(); n++) {
							if (moved[n]->uid == b->uid) {
								exists = true;
							}
						}
						if (!exists) moved.push_back(b);
					}
					
				}
			}
		}
	}
	
	//Console::Log((int)moved.size());
	for (int i = 0; i < moved.size(); i++) {
		this->add(moved[i]);
	}


	collisions.clear();
	//check collisions
	//Console::Log("Checking collsions");
	for (int i = 0; i < resolution.x; i++) {
		for (int j = 0; j < resolution.y; j++) {
			for (int k = 0; k < resolution.z; k++) {
				//std::cout << std::endl;
				for (int m = 0; m < bins->at(convertIndex(i, j, k)).size(); m++) {
					//Console::Log(bins->at(convertIndex(i, j, k))[m]->uid);
					for (int n = m + 1; n < bins->at(convertIndex(i, j, k)).size(); n++) {
						//check collision pair
						//std::cout << "Checking collision between " << bins->at(convertIndex(i, j, k))[m]->uid << " and " << bins->at(convertIndex(i, j, k))[n]->uid << ":";
						if (OrientedBoundingBox::SATCollision(bins->at(convertIndex(i, j, k))[m], bins->at(convertIndex(i, j, k))[n])) {
							//std::cout << " True" << std::endl;
							bool exists = false;
							std::pair<int, int> p2 = std::pair<int, int>(bins->at(convertIndex(i, j, k))[m]->uid, bins->at(convertIndex(i, j, k))[n]->uid);
							for (int x = 0; x < collisions.size(); x++) {
								std::pair<int, int> p1 = collisions[x];
								if ((p1.first == p2.first && p1.second == p2.second) ||
									(p1.first == p2.second && p1.second == p2.first)) {
									exists = true;
								}
							}
							if (!exists) collisions.push_back(p2);
						}
						else {
							//std::cout << " False" << std::endl;
						}
					}
				}
			}
		}
	}
}

std::vector<std::pair<int, int>> Grid::getCollisionList()
{
	return collisions;
}

void Grid::print()
{
	std::cout << "Grid Data:" << std::endl;
	std::cout << "Center:	";
	std::cout << "( " << center.x << ", " << center.y << ", " << center.z << " )" << std::endl;
	std::cout << "Half-widths:	";
	std::cout << "( " << halfWidths.x << ", " << halfWidths.y << ", " << halfWidths.z << " )" << std::endl;
	std::cout << "Resolution:	";
	std::cout << "( " << resolution.x << ", " << resolution.y << ", " << resolution.z << " )" << std::endl;
}

int Grid::convertIndex(int i, int j, int k)
{
	return i + j * resolution[0] + k * resolution[0] * resolution[1];
}

Grid::~Grid()
{
	delete bins;
}
