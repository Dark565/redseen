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

#include "engine/object/object.hh"
#include "engine/object/basic_object.hh"

namespace plane_quest::render {
class Model;
}

namespace plane_quest::engine {
class Engine;
class Model;
} // namespace plane_quest::engine

namespace plane_quest::demos::particles {

class Bullet : public engine::BasicObject {
    glm::vec3 start_pos;
    glm::vec3 velocity;
    glm::vec3 accel;
    float max_distance_sq;

  public:
    Bullet(const glm::vec3 &start_pos,
           std::shared_ptr<const engine::Model> model,
           const glm::vec3 &velocity, float accel = 0.f,
           float max_distance = INFINITY);

    Bullet(const glm::mat4 &transform,
           std::shared_ptr<const engine::Model> model,
           const glm::vec3 &velocity, float accel = 0.f,
           float max_distance = INFINITY);

    engine::ObjectUpdateResult update(engine::Engine &) override;
};

} // namespace plane_quest::demos::particles