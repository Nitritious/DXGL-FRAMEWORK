#include "CameraFPS.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include <iostream>
//Include GLFW
#include <GLFW/glfw3.h>
CameraFPS::CameraFPS()
{
}
CameraFPS::~CameraFPS()
{
}
void CameraFPS::Init(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
{
	Camera::Init(pos, target, up);
}

void CameraFPS::Reset()
{
}

void CameraFPS::Update(double dt)
{
    static const float ROTATE_SPEED = 100.0f;
    static const float ZOOM_SPEED = 10.0f;

    glm::vec3 view = glm::normalize(target - position); // calculate the new view vector
    glm::vec3 right = glm::normalize(glm::cross(view, up));

    if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT)) {
        // Implement the camera rotations
        // Calculate amount of angle to rotate
        float angle = ROTATE_SPEED * static_cast<float>(dt);
        glm::mat4 yaw = glm::rotate(
            glm::mat4(1.f), // Default identity
            glm::radians(angle), // Convert degree angle to radians
            glm::vec3(up.x, up.y, up.z)); // Use camera Up vector to rotate

        // Calculate the rotated view vector
        glm::vec3 rotatedView = yaw * glm::vec4(view, 0.f);
        target = position + rotatedView;
        isDirty = true;
    }

    else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_RIGHT)) {
        // Implement the camera rotations
        // Calculate amount of angle to rotate
        float angle = ROTATE_SPEED * static_cast<float>(dt);
        glm::mat4 yaw = glm::rotate(
            glm::mat4(1.f), // Default identity
            glm::radians(-angle), // Convert degree angle to radians
            glm::vec3(up.x, up.y, up.z)); // Use camera Up vector to rotate

        // Calculate the rotated view vector
        glm::vec3 rotatedView = yaw * glm::vec4(view, 0.f);
        target = position + rotatedView;
        isDirty = true;
    }

    this->Refresh();

    double deltaX = MouseController::GetInstance()->GetMouseDeltaX();
    float angle1 = -deltaX * ROTATE_SPEED * 0.1 * static_cast<float>(dt);
    double deltaY = MouseController::GetInstance()->GetMouseDeltaY();
    float angle2 = deltaY * ROTATE_SPEED * 0.1 * static_cast<float>(dt);
    YawMatrix = glm::rotate(
        glm::mat4(1.f),// matrix to modify
        glm::radians(angle1),// rotation angle in degree and converted to radians
        glm::vec3(up.x, up.y, up.z)// the axis to rotate along
        );
    glm::mat4 yaw = YawMatrix + glm::rotate(
        glm::mat4(1.f),// matrix to modify
        glm::radians(angle2),// rotation angle in degree and converted to radians
        glm::vec3(right.x, right.y, right.z)// the axis to rotate along
        );
    dir = YawMatrix * glm::vec4(view, 0.f);

    yawView = yaw * glm::vec4(view, 0.f);
    target = position + yawView;
    isDirty = true;
}

void Camera::Refresh() {
    if (!this->isDirty) return;

    glm::vec3 view = glm::normalize(target - position);
    glm::vec3 right = glm::normalize(glm::cross(up, view));

    // Recalculate the up vector
    //this->up = glm::normalize(glm::cross(view, right));

    this->isDirty = false;
}

void CameraFPS::Refresh() {
    Camera::Refresh();
}

bool CameraFPS::HitDetect(float x, float y, float z, float i)
{
    glm::vec3 m = position - glm::vec3(x, y, z);
    glm::vec3 view = glm::normalize(target - position);
    float b = glm::dot(m, view);
    float c = glm::dot(m, m) - i * i;
    if (c > glm::epsilon<float>() && b > glm::epsilon<float>())
    {
        return false;
    }
    float discriminant = b * b - c;
    if (discriminant < glm::epsilon<float>())
    {
        return false;
    }
    std::cout << "HIT" << std::endl;
    return true;
}