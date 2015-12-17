#include "CameraGameObject.h"
#include <iostream>
#include <glm\trigonometric.hpp>

CameraGameObject::CameraGameObject() : GameObject()
{
	//no information for drawing passed to constructor
	//position = glm::vec3(0, 16.5f, 16.8875f);
	//const float pi = glm::pi<float>();
	//yaw = pi * -5 / 16;
	//yaw = -1 * pi;
	//pitch = -0.375f;//pi * -2 / 16;
	collider = new OrientedBoundingBox(position, glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec3(0,0,0));
}

void CameraGameObject::turn(float dx, float dy)
{
	const float pi = glm::pi<float>();
	//x yaw y pitch
	yaw += dx * 0.005f;
	if (yaw > pi) {
		yaw -= 2*pi;
	}
	else if (yaw < -1 * pi) {
		yaw += 2 * pi;
	}

	pitch -= dy * 0.005f;
	if (pitch > pi / 2) {
		pitch = pi / 2;
	}
	else if (pitch < pi * -1 / 2) {
		pitch = -1 / 2 * pi;
	}

	//debug
	//std::cout << "yaw: " << yaw << " pitch: " << pitch << std::endl;
}

void CameraGameObject::setPitch(float t)
{
	pitch = t;
}

void CameraGameObject::setYaw(float t)
{
	yaw = t;
}

glm::vec3 CameraGameObject::getLocation()
{
	return GameObject::getPosition();
}

glm::vec3 CameraGameObject::getForward()
{
	float x = glm::sin(yaw)*glm::cos(pitch);
	float y = glm::sin(pitch);
	float z = -1.0*glm::cos(yaw)*glm::cos(pitch);

	return glm::vec3(x, y, z);
}

glm::vec3 CameraGameObject::getLookAt()
{
	return getLocation() + getForward();
}

glm::vec3 CameraGameObject::getUp()
{
	const float pi = glm::pi<float>();
	float x = glm::sin(yaw)*glm::cos(pitch + pi/2);
	float y = glm::sin(pitch + pi / 2);
	float z = -1.0*glm::cos(yaw)*glm::cos(pitch + pi / 2);

	return glm::vec3(x, y, z);
}

glm::vec3 CameraGameObject::getRight()
{
	return glm::cross(getForward(), getUp());
}

CameraGameObject::~CameraGameObject()
{
}
