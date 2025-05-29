#include "mesh_renderer.hh"
#include "mesh_shaders.hh"
#include "shader.hh"
#include <glad/glad.h>

namespace plane_quest::render {

MeshRenderer::MeshRenderer() : VAO(0), VBO(0), EBO(0) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Create and compile the mesh shader from external header
    shader = std::make_unique<Shader>(MESH_VERTEX_SHADER, MESH_FRAGMENT_SHADER,
                                      true);
}

MeshRenderer::~MeshRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void MeshRenderer::setupMesh(const Mesh &mesh) {
    const auto &vertices = mesh.getVertices();
    const auto &indices = mesh.getIndices();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex),
                 vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_DYNAMIC_DRAW);
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                          (void *)0);
    // texCoord
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                          (void *)offsetof(MeshVertex, texCoord));
    glBindVertexArray(0);
    lastVertexCount = vertices.size();
    lastIndexCount = indices.size();
}

void MeshRenderer::render(const Mesh &mesh, const glm::mat4 &projection,
                          const glm::mat4 &model, const glm::vec3 &color,
                          unsigned int textureID) {
    const auto &vertices = mesh.getVertices();
    const auto &indices = mesh.getIndices();
    if (vertices.size() != lastVertexCount ||
        indices.size() != lastIndexCount) {
        setupMesh(mesh);
    }
    shader->use();
    glUniform3f(glGetUniformLocation(shader->ID, "meshColor"), color.x, color.y,
                color.z);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1,
                       GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE,
                       &model[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace plane_quest::render
