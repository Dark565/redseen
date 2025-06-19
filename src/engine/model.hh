#pragma once

#include "renderer.hh"

namespace plane_quest::engine {

class Renderer;

class Model {
  protected:
    Model() = default;

  public:
    virtual bool render(Renderer &, const RenderRequest &) const = 0;
};

} // namespace plane_quest::engine