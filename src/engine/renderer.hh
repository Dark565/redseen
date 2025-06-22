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
class EventDispatcher;

/** A polymorphic structure representing API specific render requests */
struct RenderRequest {
    const Model &model;
    glm::mat4 transform;

    RenderRequest(const Model &model, const glm::mat4 &transform)
        : model(model), transform(transform) {}

    virtual ~RenderRequest() = default;
};

class Renderer : public EventObserver {
  protected:
    std::shared_ptr<Engine> engine;

  public:
    Renderer(std::shared_ptr<Engine> engine);

    virtual ~Renderer();

    virtual void init() = 0;

    bool render(const RenderRequest &);
    ObserverReturnSignal on_event(const Event &) override;
    struct IncompatibleRendererError : public std::logic_error {
        IncompatibleRendererError(const char *msg) : std::logic_error(msg) {}
    };

    static void subscribe_dispatcher(std::weak_ptr<Renderer> _this,
                                     EventDispatcher &disp);

  protected:
    virtual void update() = 0;
    /** By default Renderer iterates over all objects
    and renders them */
    virtual void render();
    virtual void present() = 0;
};

} // namespace plane_quest::engine