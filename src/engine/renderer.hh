#pragma once

#include <exception>
#include <memory>

#include <glm/glm.hpp>

#include "event_observer.hh"

namespace plane_quest::render {
class OpenGLDrawer;
}
namespace plane_quest::engine {

class Model;
class Engine;
class Event;

/** A polymorphic structure representing API specific render requests */
struct RenderRequest {
    const Model &model;
    glm::mat4 transform;

    RenderRequest(const Model &model, const glm::mat4 &transform)
        : model(model), transform(transform) {}

    virtual ~RenderRequest() = default;
};

class Renderer : public EventObserver {
    std::shared_ptr<Engine> engine;

  public:
    Renderer(std::shared_ptr<Engine> engine);

    virtual void init() = 0;
    virtual bool post_render() = 0;
    virtual void present() = 0;

    bool render(const RenderRequest &);

    struct IncompatibleRendererError : public std::logic_error {
        IncompatibleRendererError(const char *msg) : std::logic_error(msg) {}
    };
};

} // namespace plane_quest::engine