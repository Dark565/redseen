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

#ifdef DEBUG
#include <iostream>
#endif

#include <cstdlib>
#include <string_view>
#include <glm/glm.hpp>

namespace redseen::engine {

/** Base class for events */
struct Event {
    using Key = std::string;
    using KeyView = std::string_view;

    KeyView name;

    Event(const KeyView &name) : name(name) {
#ifdef DEBUG
        std::cerr << "Creating event: " << name << std::endl;
#endif
    }

    bool has_name(const KeyView &name) const { return this->name == name; }

    virtual ~Event() = default;
};

struct EngineEvent : Event {};

} // namespace redseen::engine