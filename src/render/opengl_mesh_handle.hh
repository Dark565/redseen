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

    static OpenGLMeshHandle create_from_mesh(const Mesh &mesh);
};

}; // namespace plane_quest::render