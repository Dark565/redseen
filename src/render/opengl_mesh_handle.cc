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

OpenGLMeshHandle OpenGLMeshHandle::create_from_mesh(const Mesh &mesh) {
    OpenGLMeshHandle handle;
    handle.load_mesh(mesh);
    return handle;
}

} // namespace plane_quest::render