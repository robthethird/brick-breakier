#define GLM_FORCE_RADIANS // for removing degrees error message

#include "GameObject.h"
#include "shaderHelpers.h"
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <random>
#include "GameData.h"


GameObject::GameObject() {
	active = false;
	doDelete = false;
	scale = glm::vec3(1, 1, 1);
	rotation = glm::vec3(0, 0, 1);
	velocity = glm::vec3(0, 0, 0);
	force = glm::vec3(0, 0, 0);
	//srand((unsigned int)NULL);
	position = glm::vec3(0, 0, 0);
	angularVel = 0;
	angle = 0;

	active = true; //todo
}



/*
Takes a shape to display, the shader program
*/
GameObject::GameObject(Shape * inShape)
{
	doDelete = false;
	shape = inShape;
	
	texID = GameData::instance().getTexture("stone");

	glm::vec3 min = glm::vec3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	glm::vec3 max = glm::vec3(std::numeric_limits<float>::infinity()*-1, std::numeric_limits<float>::infinity()*-1, std::numeric_limits<float>::infinity()*-1);
	for (int i = 0; i < shape->vertices()->size(); i++)
	{
		if (shape->vertices()->at(i).x < min.x) {
			min.x = shape->vertices()->at(i).x;
		}
		if (shape->vertices()->at(i).y < min.y) {
			min.y = shape->vertices()->at(i).y;
		}
		if (shape->vertices()->at(i).z < min.z) {
			min.z = shape->vertices()->at(i).z;
		}

		if (shape->vertices()->at(i).x > max.x) {
			max.x = shape->vertices()->at(i).x;
		}
		if (shape->vertices()->at(i).y > max.y) {
			max.y = shape->vertices()->at(i).y;
		}
		if (shape->vertices()->at(i).z > max.z) {
			max.z = shape->vertices()->at(i).z;
		}
	}

	glm::vec3 tCenter = (min + max) / 2.f;
	collider = new OrientedBoundingBox(tCenter, glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec3(tCenter.x-min.x, tCenter.y-min.y, tCenter.z-min.z));

	
	scale = glm::vec3(1, 1, 1);
	rotation = glm::vec3(0, 0, 1);
	velocity = glm::vec3(0, 0, 0);
	force = glm::vec3(0, 0, 0);
	position = glm::vec3(0, 0, 0);
	angularVel = 0;
	angle = 0;

	active = true; //todo
}

void GameObject::renew()
{
	
	
}

/*
Applies friction, then uses linear motion
to move the GameObject, and calculates the transformation
matrix to be used when drawing
*/
void GameObject::update(float dt)
{


	velocity += force * dt;
	position += velocity * dt;
	angle += angularVel * dt;
	force = glm::vec3(0, 0, 0);

	collider->translate(position);
	collider->scale(scale);
	world = glm::translate(position) * glm::scale(scale) * glm::rotate(angle, rotation);

}

/*
Applies a given force to an GameObject by adding it to the velocity
*/
void GameObject::applyForce(glm::vec3 pForce)
{
	force += pForce;
}

void GameObject::collideWithNormal(glm::vec3 normal) {
	normal = glm::normalize(normal);
	glm::vec3 vNorm = normal * (glm::dot(velocity, normal));
	velocity -= vNorm * 2.0f;
}

/*
sets pos to mouse
*/
void GameObject::setMousePos(glm::vec2 inMouse)
{
	position.x = inMouse.x;
	position.y = inMouse.y;
	world = glm::translate(position) * glm::scale(scale) * glm::rotate(angle, rotation); //todo dirty flag
}

/*
Set velocity
*/
void GameObject::setVelocity(glm::vec3 pVelocity)
{
	velocity = pVelocity;
}

/*
get current  velocity
*/
glm::vec3 GameObject::getVelocity()
{
	return velocity;
}

/*
get current position
*/
glm::vec3 GameObject::getPosition()
{
	return position;
}

void GameObject::setPosition(glm::vec3 p)
{
	position = p;
}

glm::vec3 GameObject::getScale()
{
	return scale;
}

void GameObject::setScale(glm::vec3 s)
{
	scale = s;
}

OrientedBoundingBox * GameObject::getCollider()
{
	return collider;
}

void GameObject::setTexture(const char * name)
{

	texID = GameData::instance().getTexture(name);
}

/*
Draws the shape object using the matrix calculated during update()
*/
void GameObject::draw()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	shape->Draw(world);
}


/*
Mark for deletion
*/
void GameObject::dispose()
{
	doDelete = true;
	collider->doDelete = true;
}

/*
Deletes
*/
GameObject::~GameObject()
{
	delete collider;
	//delete shape;
}