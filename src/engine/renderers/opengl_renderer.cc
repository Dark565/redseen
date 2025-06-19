#include "opengl_renderer.hh"

#include "engine/event_observer.hh"
#include "engine/event.hh"
#include "engine/renderer.hh"
#include "render/opengl_drawer.hh"
#include "render/mesh_renderer.hh"
#include "render/model.hh"

namespace plane_quest::engine::renderers {

OpenGLRenderer::OpenGLRenderer(std::shared_ptr<render::OpenGLDrawer> ogl_drawer)
    : ogl_drawer(ogl_drawer),
      mesh_renderer(std::make_unique<render::MeshRenderer>()) {}

ObserverReturnSignal OpenGLRenderer::on_event(const Event &ev) {
    if (ev.type != EventType::TICK)
        return ObserverReturnSignal::CONTINUE;

    post_render();
    present();

    return ObserverReturnSignal::CONTINUE;
}

void OpenGLRenderer::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool OpenGLRenderer::post_render() { return true; }
void OpenGLRenderer::present() { ogl_drawer->present(); }

bool OpenGLRenderer::render(const render::Model &model,
                            const glm::mat4 &transform) {

    model.render(*mesh_renderer, transform);
}

const std::unique_ptr<render::MeshRenderer> &
OpenGLRenderer::get_mesh_renderer() const {
    return mesh_renderer;
}

} // namespace plane_quest::engine::renderers