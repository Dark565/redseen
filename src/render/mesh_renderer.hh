#ifndef MESH_RENDERER_HH
#define MESH_RENDERER_HH

#include "mesh.hh"
#include "render/opengl_mesh_handle.hh"
#include "shader.hh"
#include <glm/glm.hpp>
#include <memory>

namespace plane_quest::render {

class MeshRenderer {
  public:
    MeshRenderer();
    ~MeshRenderer();

    void render(const OpenGLMeshHandle &mesh, const glm::mat4 &projection,
                const glm::mat4 &model, const glm::vec3 &color,
                unsigned int textureID);

  private:
    std::unique_ptr<Shader> shader;
};

} // namespace plane_quest::render

#endif
