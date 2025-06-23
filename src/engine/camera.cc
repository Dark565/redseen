/*
 *  Copyright (C) 2025 Grzegorz Kociołek (grzegorz.kclk@gmail.com)
 *
 *  This file is a part of RedSeen; a 3D game engine.
 *
 *  RedSeen is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RedSeen is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "camera.hh"
#include <glm/gtc/matrix_transform.hpp>

namespace redseen::engine {

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, float yaw,
               float pitch, float aspectRatio)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch),
      aspectRatio(aspectRatio) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const { return view; }

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane,
                            farPlane);
}

void Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
}

void Camera::setPosition(const glm::vec3 &newPosition) {
    position = newPosition;
    updateView();
}

void Camera::setRotation(float newYaw, float newPitch) {
    yaw = newYaw;
    pitch = newPitch;
    updateCameraVectors();
}

void Camera::move(const glm::vec3 &offset) {
    position += offset;
    updateView();
}

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

void Camera::updateView() {
    view = glm::lookAt(position, position + front, up);
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    updateView();
}

} // namespace redseen::engine