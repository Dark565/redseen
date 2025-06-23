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

#include "mesh_renderer.hh"
#include "opengl_mesh_handle.hh"
#include "mesh_shaders.hh"
#include "shader.hh"
#include <glad/glad.h>

namespace plane_quest::render {

MeshRenderer::MeshRenderer() {
    // Create and compile the mesh shader from external header
    shader = std::make_unique<Shader>(MESH_VERTEX_SHADER, MESH_FRAGMENT_SHADER,
                                      true);
}

MeshRenderer::~MeshRenderer() = default;

void MeshRenderer::render(const OpenGLMeshHandle &mesh,
                          const glm::mat4 &projection, const glm::mat4 &model,
                          const glm::vec3 &color, unsigned int textureID,
                          const glm::vec3 &lightPosition) {
    // TODO: Expand lightning implementation

    shader->use();

    // Set uniforms
    glUniform3f(glGetUniformLocation(shader->ID, "meshColor"), color.x, color.y,
                color.z);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1,
                       GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE,
                       &model[0][0]);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shader->ID, "meshTextures"), 0);

    // Lightning
    glUniform3f(glGetUniformLocation(shader->ID, "lightPosition"),
                lightPosition.x, lightPosition.y, lightPosition.z);
    // TODO: Make it not hardcoded
    glUniform3f(glGetUniformLocation(shader->ID, "lightColor"), 1.0, 1.0, 1.0);
    glUniform3f(glGetUniformLocation(shader->ID, "ambientColor"), 0.3, 0.3,
                0.3);
    glUniform1f(glGetUniformLocation(shader->ID, "Kc"), 1.0);
    glUniform1f(glGetUniformLocation(shader->ID, "K1"), 0.09f);
    glUniform1f(glGetUniformLocation(shader->ID, "Kq"), 0.032f);

    // Bind mesh and draw
    glBindVertexArray(mesh.get_vao());
    glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace plane_quest::render
