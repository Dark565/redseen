#pragma once

#include <glm/glm.hpp>
#include "renderer.hh"

namespace plane_quest::engine {

class Renderer;

class Model {
  protected:
    Model() = default;

  public:
    virtual bool render(Renderer &, const RenderRequest &,
                        const glm::mat4 &view) const = 0;
};

} // namespace plane_quest::engine