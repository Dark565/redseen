#ifndef MESH_RENDERER_HH
#define MESH_RENDERER_HH

#include "mesh.hh"
#include "shader.hh"
#include <glm/glm.hpp>
#include <memory>

namespace plane_quest::render {

class MeshRenderer {
  public:
    MeshRenderer();
    ~MeshRenderer();

    void render(const Mesh &mesh, const glm::mat4 &projection,
                const glm::mat4 &model, const glm::vec3 &color,
                unsigned int textureID);

  private:
    unsigned int VAO, VBO, EBO;
    size_t lastVertexCount = 0;
    size_t lastIndexCount = 0;
    void setupMesh(const Mesh &mesh);
    std::unique_ptr<Shader> shader;
};

} // namespace plane_quest::render

#endif
