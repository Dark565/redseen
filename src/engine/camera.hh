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

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace redseen::engine {

class Camera {
  public:
    Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 3.0f),
           const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, float pitch = 0.0f, float aspectRatio = 1.0f);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void setPosition(const glm::vec3 &position);
    void setRotation(float yaw, float pitch);
    void move(const glm::vec3 &offset);
    void rotate(float yawOffset, float pitchOffset);
    void setAspectRatio(float aspectRatio);

    const glm::vec3 &getPosition() const { return position; }
    const glm::vec3 &getFront() const { return front; }
    const glm::vec3 &getUp() const { return up; }
    const glm::vec3 &getRight() const { return right; }
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    float getAspectRatio() const { return aspectRatio; }

  private:
    void updateView();
    void updateCameraVectors();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    glm::mat4 view;

    float yaw;
    float pitch;

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    float aspectRatio;
};

} // namespace redseen::engine