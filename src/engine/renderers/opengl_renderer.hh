#pragma once

#include <memory>

#include "engine/renderer.hh"
#include "engine/event_observer.hh"

namespace plane_quest::render {
class OpenGLDrawer;
class MeshRenderer;
class Model;
} // namespace plane_quest::render
namespace plane_quest::engine {
class Event;

namespace renderers {

struct OpenGLRenderRequest : RenderRequest {
    std::shared_ptr<render::Model> model;
};

class OpenGLRenderer : public engine::Renderer {
    std::shared_ptr<render::OpenGLDrawer> ogl_drawer;
    std::unique_ptr<render::MeshRenderer> mesh_renderer;

  public:
    OpenGLRenderer(std::shared_ptr<Engine>,
                   std::shared_ptr<render::OpenGLDrawer>);

    void init() override;
    void update() override;
    void present() override;

    bool render(const render::Model &, const glm::mat4 &transform);

    const std::unique_ptr<render::MeshRenderer> &get_mesh_renderer() const;
};

} // namespace renderers
} // namespace plane_quest::engine
