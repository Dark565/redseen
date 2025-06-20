#include "renderer.hh"

#include "engine/engine.hh"
#include "model.hh"

namespace plane_quest::engine {

Renderer::Renderer(std::shared_ptr<Engine> engine) : engine(engine) {}

bool Renderer::render(const RenderRequest &req) {
    return req.model.render(*this, req,
                            engine->get_player_camera().getViewMatrix());
}
} // namespace plane_quest::engine