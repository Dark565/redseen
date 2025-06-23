#include "opengl_renderer.hh"

#include "engine/engine.hh"
#include "engine/renderer.hh"
#include "render/opengl_drawer.hh"
#include "render/mesh_renderer.hh"
#include "render/model.hh"

#include <glad/glad.h>

namespace plane_quest::engine::renderers {

OpenGLRenderer::OpenGLRenderer(std::shared_ptr<Engine> engine,
                               std::shared_ptr<render::OpenGLDrawer> ogl_drawer)
    : Renderer(engine), ogl_drawer(ogl_drawer),
      mesh_renderer(std::make_unique<render::MeshRenderer>()) {}

void OpenGLRenderer::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderer::update() { ogl_drawer->clear(0, 0, 0); }

void OpenGLRenderer::present() { ogl_drawer->present(); }

bool OpenGLRenderer::render(const render::Model &model,
                            const glm::mat4 &transform,
                            const glm::vec3 &lightPos) {

    model.render(*mesh_renderer,
                 engine->get_player_camera().getProjectionMatrix(), transform,
                 lightPos);

    return true;
}

const std::unique_ptr<render::MeshRenderer> &
OpenGLRenderer::get_mesh_renderer() const {
    return mesh_renderer;
}

} // namespace plane_quest::engine::renderers
