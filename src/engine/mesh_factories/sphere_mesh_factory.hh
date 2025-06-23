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

#include "engine/mesh_factory.hh"

namespace plane_quest::engine::mesh_factories {

class Sphere : public MeshFactory {
  public:
    Sphere(float radius = 1.0f, int sectors = 8, int stacks = 8);
    ~Sphere() override = default;

    // Create a sphere mesh with the configured parameters
    std::unique_ptr<render::Mesh> create_mesh() override;

  private:
    float radius;
    int sectors;
    int stacks;
};

} // namespace plane_quest::engine::mesh_factories