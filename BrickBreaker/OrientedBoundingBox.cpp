#include "OrientedBoundingBox.h"
#include "GameData.h"
#include <iostream>

OrientedBoundingBox::OrientedBoundingBox(glm::vec3 c, glm::vec3 u1, glm::vec3 u2, glm::vec3 u3, glm::vec3 es)
{
	uid = GameData::instance().getUniqueID();
	center = c;
	//u = new glm::vec3[3];
	u.push_back(u1);
	u.push_back(u2);
	u.push_back(u3);
	orig_e = e = es;
	doDelete = false;
}

bool OrientedBoundingBox::SATCollision(OrientedBoundingBox * b1, OrientedBoundingBox * b2)
{

	glm::vec3 T = b2->center - b1->center;

	//3 axes for box 1
	for (int i = 0; i < 3; i++) {
		glm::vec3 L = b1->u[i];
		float sum = 0;
		for (int j = 0; j < 3; j++) {
			sum += glm::abs(glm::dot((b1->u[j] * b1->e[j]), L));
			sum += glm::abs(glm::dot((b2->u[j] * b2->e[j]), L));
		}

		if (glm::abs(glm::dot(T, L)) > sum) {
			//axis is separating
			return false;
		}
	}

	//3 axes for box 2
	for (int i = 0; i < 3; i++) {
		glm::vec3 L = b2->u[i];
		float sum = 0;
		for (int j = 0; j < 3; j++) {
			sum += glm::abs(glm::dot((b1->u[j] * b1->e[j]), L));
			sum += glm::abs(glm::dot((b2->u[j] * b2->e[j]), L));
		}

		if (glm::abs(glm::dot(T, L)) > sum) {
			//axis is separating
			return false;
		}
	}

	//9 edge pairs
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			glm::vec3 L = glm::cross(b1->u[i], b2->u[j]);
			float sum = 0;
			for (int k = 0; k < 3; k++) {
				sum += glm::abs(glm::dot((b1->u[k] * b1->e[k]), L));
				sum += glm::abs(glm::dot((b2->u[k] * b2->e[k]), L));
			}

			if (glm::abs(glm::dot(T, L)) > sum) {
				//axis is separating
				return false;
			}
		}
	}

	return true;
}
glm::vec3 OrientedBoundingBox::MinimumTranslationVector(OrientedBoundingBox * b1, OrientedBoundingBox * b2)
{

	glm::vec3 T = b2->center - b1->center;

	glm::vec3 minAxis;
	float minAmt = FLT_MAX;
	//3 axes for box 1
	for (int i = 0; i < 3; i++) {
		glm::vec3 L = b1->u[i];
		float sum = 0;
		for (int j = 0; j < 3; j++) {
			sum += glm::abs(glm::dot((b1->u[j] * b1->e[j]), L));
			sum += glm::abs(glm::dot((b2->u[j] * b2->e[j]), L));
		}

		if (glm::abs(glm::dot(T, L)) > sum) {
			//axis is separating
			return glm::vec3(0, 0, 0);
		}
		else {
			if (sum - glm::abs(glm::dot(T, L)) < minAmt) {
				minAmt = sum - glm::abs(glm::dot(T, L));
				minAxis = L;
				if (glm::dot(T, L) > 0) {
					minAxis *= -1;
				}
			}
		}
	}

	//3 axes for box 2
	for (int i = 0; i < 3; i++) {
		glm::vec3 L = b2->u[i];
		float sum = 0;
		for (int j = 0; j < 3; j++) {
			sum += glm::abs(glm::dot((b1->u[j] * b1->e[j]), L));
			sum += glm::abs(glm::dot((b2->u[j] * b2->e[j]), L));
		}

		if (glm::abs(glm::dot(T, L)) > sum) {
			//axis is separating
			return glm::vec3(0, 0, 0);
		}
		else {
			if (sum - glm::abs(glm::dot(T, L)) < minAmt) {
				minAmt = sum - glm::abs(glm::dot(T, L));
				minAxis = L;
				if (glm::dot(T, L) > 0) {
					minAxis *= -1;
				}
			}
		}
	}

	//9 edge pairs
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			glm::vec3 L = glm::cross(b1->u[i], b2->u[j]);
			float sum = 0;
			for (int k = 0; k < 3; k++) {
				sum += glm::abs(glm::dot((b1->u[k] * b1->e[k]), L));
				sum += glm::abs(glm::dot((b2->u[k] * b2->e[k]), L));
			}

			if (glm::abs(glm::dot(T, L)) > sum) {
				//axis is separating
				return glm::vec3(0, 0, 0);
			}
			else {
				if (sum - glm::abs(glm::dot(T, L)) < minAmt && sum > 0) {
					minAmt = sum - glm::abs(glm::dot(T, L));
					minAxis = L;
					if (glm::dot(T, L) > 0) {
						minAxis *= -1;
					}
				}
			}
		}
	}
	minAxis = glm::normalize(minAxis);
	minAxis *= minAmt;
	return minAxis;
}


void OrientedBoundingBox::translate(glm::vec3 p)
{
	if (p != center) {
		center = p;
		moved = true;
	}
}

void OrientedBoundingBox::scale(glm::vec3 s)
{
	glm::vec3 newScale = glm::vec3(orig_e.x * s.x, orig_e.y * s.y, orig_e.z * s.z);
	if (newScale != e) {
		e = newScale;
		moved = true;
	}
}

void OrientedBoundingBox::print()
{
	std::cout << "OBB #" << uid << " Data" << std::endl;
	std::cout << "Center:	";
	std::cout << "( " << center.x << ", " << center.y << ", " << center.z << " )" << std::endl;
	std::cout << "Axis u0:	";
	std::cout << "( " << u[0].x << ", " << u[0].y << ", " << u[0].z << " )" << std::endl;
	std::cout << "Axis u1:	";
	std::cout << "( " << u[1].x << ", " << u[1].y << ", " << u[1].z << " )" << std::endl;
	std::cout << "Axis u2:	";
	std::cout << "( " << u[2].x << ", " << u[2].y << ", " << u[2].z << " )" << std::endl;
	std::cout << "Half-widths:	";
	std::cout << "( " << e.x << ", " << e.y << ", " << e.z << " )" << std::endl;
}

OrientedBoundingBox::~OrientedBoundingBox()
{
	//delete[] u;
}
