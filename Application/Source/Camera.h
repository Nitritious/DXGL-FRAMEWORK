#pragma once
// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;

	Camera();
	~Camera();

	void Init(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);
	void Reset();
	void Update(double dt);
protected:
	bool isDirty; // Initialise this in the init function
	virtual void Refresh();
};
#endif


