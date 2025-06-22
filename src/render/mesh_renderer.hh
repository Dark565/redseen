#ifndef MESH_RENDERER_HH
#define MESH_RENDERER_HH

#include <glm/glm.hpp>
#include <memory>

#include "render/opengl_mesh_handle.hh"
#include "shader.hh"

namespace plane_quest::render {

class MeshRenderer {
  public:
    MeshRenderer();
    ~MeshRenderer();

    void render(const OpenGLMeshHandle &mesh, const glm::mat4 &projection,
                const glm::mat4 &model, const glm::vec3 &color,
                unsigned int textureID, const glm::vec3 &lightPosition);

  private:
    std::unique_ptr<Shader> shader;
};

} // namespace plane_quest::render

#endif
