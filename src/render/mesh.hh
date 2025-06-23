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

#ifndef MESH_HH
#define MESH_HH

#include <concepts>
#include <glm/glm.hpp>
#include <type_traits>
#include <vector>

namespace plane_quest::render {

struct MeshVertex {
    glm::vec3 position;
    glm::vec2 texCoord;
};

class Mesh {
  public:
    using VertexVec = std::vector<MeshVertex>;
    using IndexVec = std::vector<unsigned int>;

  private:
    VertexVec vertices;
    IndexVec indices;

  public:
    template <class V, class I>
        requires std::same_as<std::remove_cvref_t<V>, VertexVec> &&
                     std::same_as<std::remove_cvref_t<I>, IndexVec>
    Mesh(V &&vertices, I &&indices)
        : vertices(std::forward<V>(vertices)),
          indices(std::forward<I>(indices)) {}

    const VertexVec &get_vertices() const;
    const IndexVec &get_indices() const;
    const std::size_t get_index_count() const;
};

} // namespace plane_quest::render

#endif
