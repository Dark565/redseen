#include "opengl_model.hh"
#include "engine/renderer.hh"
#include "engine/renderers/opengl_renderer.hh"

namespace plane_quest::engine::model {
OpenGLModel::OpenGLModel(std::shared_ptr<render::Model> model) : model(model) {}

bool OpenGLModel::render(Renderer &renderer, const RenderRequest &req,
                         const glm::mat4 &view) const {
    auto ogl_renderer = dynamic_cast<renderers::OpenGLRenderer *>(&renderer);
    if (ogl_renderer == nullptr)
        throw Renderer::IncompatibleRendererError(
            "OpenGLModel can only be rendered with OpenGLRenderer");

    return ogl_renderer->render(*model, view * req.transform);
}

} // namespace plane_quest::engine::model