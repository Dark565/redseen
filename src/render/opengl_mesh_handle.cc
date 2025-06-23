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

#include "opengl_mesh_handle.hh"

#include <glad/glad.h>

#include "mesh.hh"

namespace plane_quest::render {

OpenGLMeshHandle::OpenGLMeshHandle() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

OpenGLMeshHandle::OpenGLMeshHandle(unsigned int VAO, unsigned int VBO,
                                   unsigned int EBO, std::size_t index_count)
    : VAO(VAO), VBO(VBO), EBO(EBO), index_count(index_count) {}

OpenGLMeshHandle::~OpenGLMeshHandle() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

OpenGLMeshHandle &OpenGLMeshHandle::operator=(OpenGLMeshHandle &&other) {
    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
    index_count = other.index_count;
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.index_count = 0;
    return *this;
}

OpenGLMeshHandle::OpenGLMeshHandle(OpenGLMeshHandle &&other) {
    *this = std::move(other);
}

unsigned int OpenGLMeshHandle::get_vao() const { return VAO; }

unsigned int OpenGLMeshHandle::get_vbo() const { return VBO; }

unsigned int OpenGLMeshHandle::get_ebo() const { return EBO; }

std::size_t OpenGLMeshHandle::get_index_count() const { return index_count; }

void OpenGLMeshHandle::load_mesh(const Mesh &mesh) {
    const auto &vertices = mesh.get_vertices();
    const auto &indices = mesh.get_indices();

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex),
                 vertices.data(), GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // Set up vertex attributes
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                          (void *)0);

    // Texture coordinate attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                          (void *)offsetof(MeshVertex, texCoord));

    glBindVertexArray(0);
    index_count = indices.size();
}

std::unique_ptr<OpenGLMeshHandle>
OpenGLMeshHandle::create_from_mesh(const Mesh &mesh) {
    auto handle = std::make_unique<OpenGLMeshHandle>();
    handle->load_mesh(mesh);
    return handle;
}

} // namespace plane_quest::render