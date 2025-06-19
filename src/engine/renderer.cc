#include "renderer.hh"

#include "model.hh"

namespace plane_quest::engine {
bool Renderer::render(const RenderRequest &req) {
    return req.model.render(*this, req);
}
} // namespace plane_quest::engine