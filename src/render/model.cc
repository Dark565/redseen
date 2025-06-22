#include "model.hh"
#include "shader.hh"
#include <memory>

namespace plane_quest::render {

void Model::render(MeshRenderer &renderer, const glm::mat4 &projection,
                   const glm::mat4 &parentTransform,
                   const glm::vec3 &lightPos) const {
    // TODO: Add shader usage
    glm::mat4 modelMatrix = parentTransform * transform_;
    renderer.render(*mesh_, projection, modelMatrix, color_, textureID_,
                    lightPos);
}

} // namespace plane_quest::render
