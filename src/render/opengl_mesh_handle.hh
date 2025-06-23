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

#include "mesh.hh"

namespace plane_quest::render {

class Mesh;

/** A class encapsulating OpenGL handles to objects related to mesh */
class OpenGLMeshHandle {
    unsigned int VAO, VBO, EBO;
    std::size_t index_count;

  public:
    /** Allocate new vao, vbo and ebo. */
    OpenGLMeshHandle();

    /** Take ownership of some existing vao, vbo and ebo. */
    OpenGLMeshHandle(unsigned int VAO, unsigned int VBO, unsigned int EBO,
                     std::size_t index_count);

    OpenGLMeshHandle(const OpenGLMeshHandle &) = delete;
    OpenGLMeshHandle &operator=(const OpenGLMeshHandle &) = delete;

    OpenGLMeshHandle(OpenGLMeshHandle &&);
    OpenGLMeshHandle &operator=(OpenGLMeshHandle &&);

    ~OpenGLMeshHandle();

    void load_mesh(const Mesh &mesh);

    unsigned int get_vao() const;
    unsigned int get_vbo() const;
    unsigned int get_ebo() const;
    std::size_t get_index_count() const;

    OpenGLMeshHandle duplicate() const;

    static std::unique_ptr<OpenGLMeshHandle> create_from_mesh(const Mesh &mesh);
};

}; // namespace plane_quest::render