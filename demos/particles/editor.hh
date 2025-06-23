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
#include <unordered_set>

#include "engine/event_observer.hh"

namespace plane_quest::engine {
class Engine;
class Object;
}; // namespace plane_quest::engine

namespace plane_quest::demos::particles {
class Editor : engine::EventObserver {
    std::shared_ptr<engine::Engine> engine;
    std::unordered_set<std::shared_ptr<engine::Object>> objects;

    Editor(std::shared_ptr<engine::Engine> engine);

  public:
    Editor(Editor &&editor) = default;
    Editor &operator=(Editor &&engine) = default;

    friend class Engine;
};
} // namespace plane_quest::demos::particles