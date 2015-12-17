#pragma once
#include "GameObject.h"
class CameraGameObject :
	public GameObject
{
public:
	CameraGameObject();
	~CameraGameObject();

	void turn(float dx, float dy);
	glm::vec3 getLocation();
	glm::vec3 getForward();
	glm::vec3 getLookAt();
	glm::vec3 getUp();
	glm::vec3 getRight();

	void setPitch(float);
	void setYaw(float);

private:
	float pitch;
	float yaw;
};

