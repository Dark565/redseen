#pragma once

#include <memory>
#include "engine/model.hh"
#include "engine/renderer.hh"
#include "render/model.hh"

namespace plane_quest::engine {
class Renderer;
}

namespace plane_quest::engine::model {
class OpenGLModel : public Model {
    std::shared_ptr<render::Model> model;

  public:
    OpenGLModel(std::shared_ptr<render::Model>);

    bool render(Renderer &, const RenderRequest &) const override;
};
} // namespace plane_quest::engine::model