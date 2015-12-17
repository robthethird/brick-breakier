#pragma once

#include "Shape.h"
#include "OrientedBoundingBox.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

class GameObject
{
protected:	

	bool active;
	float lifetime;

	Shape * shape;
	OrientedBoundingBox * collider;
	GLuint texID;

	//the transformation information
	glm::vec3  position;
	glm::vec3  velocity;
	glm::vec3  scale;
	glm::vec3  rotation;

	glm::vec3  force;

	//current position of the mouse
	glm::vec3  mousePos;

	//angle for rotation
	float angle;
	float angularVel;

	//world matrix
	glm::mat4 world;

public:
	GameObject();
	GameObject(Shape * shape);
	void update(float dt);
	void setMousePos(glm::vec2 inMouse);
	
	void applyForce(glm::vec3 force);
	void draw();
	void renew();
	void dispose();
	void setTexture(const char *);
	void collideWithNormal(glm::vec3 norm);

	//get-set
	glm::vec3 getPosition();
	void setPosition(glm::vec3 p);
	glm::vec3 getScale();
	void setScale(glm::vec3 s);
	glm::vec3 getVelocity();
	void setVelocity(glm::vec3 vel);

	OrientedBoundingBox * getCollider();
	
	bool doDelete;

	virtual ~GameObject();
};

