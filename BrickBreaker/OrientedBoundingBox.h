#pragma once
#include <glm\glm.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>

class OrientedBoundingBox
{
public:
	OrientedBoundingBox(glm::vec3 c, glm::vec3 u1, glm::vec3 u2, glm::vec3 u3, glm::vec3 es);

	static bool SATCollision(OrientedBoundingBox * b1, OrientedBoundingBox * b2);
	static glm::vec3 MinimumTranslationVector(OrientedBoundingBox * b1, OrientedBoundingBox * b2);

	void translate(glm::vec3);
	void scale(glm::vec3);
	//void rotate(glm::vec3);
	void print();

	~OrientedBoundingBox();

	int uid;
	glm::vec3 center;
	std::vector<glm::vec3> u;
	glm::vec3 e;
	bool doDelete;
	bool moved;

private:
	glm::vec3 orig_e;
};

