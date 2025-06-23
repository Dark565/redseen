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
#include <string_view>

#include "engine/geometry.hh"

namespace plane_quest::engine {

class Engine;

enum class ObjectUpdateResult {
    /** Normal update */
    NORMAL,
    /** Destroy the object */
    DESTROY
};

class Object {
  protected:
    Object() = default;

  public:
    virtual Position3f get_pos() const = 0;
    virtual void set_pos(const Position3f &pos) = 0;
    /** Update the state of the object.
    Called by the ObjectManager on each TICK */
    virtual ObjectUpdateResult update(Engine &) = 0;

    /** Render the object. Called by Renderer. */
    virtual bool render(Engine &, const glm::vec3 &lightPos) = 0;

    friend class ObjectManager;
};
} // namespace plane_quest::engine