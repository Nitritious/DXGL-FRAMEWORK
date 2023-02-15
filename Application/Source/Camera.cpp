#include "Camera.h"
// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

void Camera::Reset()
{
}

void Camera::Update(double dt)
{
}

