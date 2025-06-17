#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace plane_quest::engine {

class Camera {
  public:
    Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 3.0f),
           const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, float pitch = 0.0f);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    void setPosition(const glm::vec3 &position);
    void setRotation(float yaw, float pitch);
    void move(const glm::vec3 &offset);
    void rotate(float yawOffset, float pitchOffset);

    const glm::vec3 &getPosition() const { return position; }
    const glm::vec3 &getFront() const { return front; }
    const glm::vec3 &getUp() const { return up; }
    const glm::vec3 &getRight() const { return right; }
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }

  private:
    void updateCameraVectors();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};

} // namespace plane_quest::engine