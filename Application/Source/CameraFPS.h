#ifndef CAMERA_FPS_H
#define CAMERA_FPS_H
#include "Camera.h"
class CameraFPS :
	public Camera
{
public:
	CameraFPS();
	~CameraFPS();
	void Init(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);
	void Reset();
	void Update(double dt);
	bool HitDetect(float x, float y, float z, float i);
	glm::vec3 yawView;
	glm::mat4 YawMatrix;
	glm::vec3 dir;
private:
	void Refresh();
};
#endif
