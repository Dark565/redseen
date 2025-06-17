#include "camera.hh"
#include <glm/gtc/matrix_transform.hpp>

namespace plane_quest::engine {

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, float yaw,
               float pitch)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane,
                            farPlane);
}

void Camera::setPosition(const glm::vec3 &newPosition) {
    position = newPosition;
}

void Camera::setRotation(float newYaw, float newPitch) {
    yaw = newYaw;
    pitch = newPitch;
    updateCameraVectors();
}

void Camera::move(const glm::vec3 &offset) { position += offset; }

void Camera::rotate(float yawOffset, float pitchOffset) {
    yaw += yawOffset;
    pitch += pitchOffset;

    // Constrain pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

} // namespace plane_quest::engine