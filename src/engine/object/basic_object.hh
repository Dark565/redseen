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

#include <memory>

#include "object.hh"

namespace plane_quest::engine {

class Model;
class Engine;

/** External objects should derive from this class */
class BasicObject : public Object {
    glm::mat4 transform;
    std::shared_ptr<const Model> model;

  public:
    BasicObject(const glm::mat4 &, std::shared_ptr<const Model>);
    BasicObject(const Position3f &, std::shared_ptr<const Model>);

    std::shared_ptr<const Model> get_model() const;
    void set_model(std::shared_ptr<const Model> model);

    glm::mat4 get_transform() const;
    void set_transform(const glm::mat4 &transform);

    Position3f get_pos() const override;
    void set_pos(const Position3f &pos) override;

    ObjectUpdateResult update(Engine &) override;
    bool render(Engine &, const glm::vec3 &lightPos) override;
};

} // namespace plane_quest::engine