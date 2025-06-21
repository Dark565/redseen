#include "engine.hh"

#include <memory>
#include <glad/glad.h>

#include "camera.hh"
#include "engine/event_dispatcher.hh"
#include "engine/event_observer.hh"
#include "engine/object/object.hh"
#include "engine/texture_manager.hh"
namespace plane_quest::engine {

class EventLoop;

const std::shared_ptr<TextureManager> &Engine::get_texture_manager() const {
    return texture_manager;
}

const std::shared_ptr<ObjectManager> &Engine::get_object_manager() const {
    return object_manager;
}

const std::shared_ptr<Renderer> &Engine::get_renderer() const {
    return renderer;
}

const std::unique_ptr<EventDispatcher> &Engine::get_event_dispatcher() const {
    return event_dispatcher;
}

const std::unique_ptr<EventDispatcher> &
Engine::get_internal_event_dispatcher() const {
    return internal_event_dispatcher;
}

void Engine::set_renderer(const std::shared_ptr<Renderer> &renderer) {
    this->renderer = renderer;
}

Camera &Engine::get_player_camera() { return player_camera; }

const Camera &Engine::get_player_camera() const { return player_camera; }

void Engine::init_opengl() {
    // Setup OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::init() {
    event_dispatcher = std::make_unique<EventDispatcher>();
    internal_event_dispatcher = std::make_unique<EventDispatcher>();
    object_manager = std::make_shared<ObjectManager>(shared_from_this());
    texture_manager = std::make_shared<TextureManager>();
}

bool Engine::run() {
    bool ret;

    get_renderer()->init();

    subscribe_dispatcher(*internal_event_dispatcher);
    get_renderer()->subscribe_dispatcher(get_renderer(),
                                         *internal_event_dispatcher);

    get_object_manager()->subscribe_dispatcher(get_object_manager(),
                                               *internal_event_dispatcher);

    while (1) {
    }

    return true;
}

std::shared_ptr<Engine> Engine::create() {
    struct SharedHelper : public Engine {};
    std::shared_ptr<Engine> engine_ = std::make_shared<SharedHelper>();
    engine_->init();
    return engine_;
}

ObserverReturnSignal Engine::on_event(const Event &ev) {
    if (ev.has_name(engine_events::TICK)) {
        if (tick_state.state != EngineTickState::START)
            return ObserverReturnSignal::CONTINUE;

        event_dispatcher->queue_event(
            std::make_shared<Event>(engine_events::UPDATE));
    } else if (ev.has_name(engine_events::OBJECT_UPDATE_DONE)) {
        tick_state.object_update_done = 1;
    } else if (ev.has_name(engine_events::RENDER_UPDATE_DONE)) {
        tick_state.render_update_done = 1;
    } else if (ev.has_name(engine_events::RENDER_DONE)) {
        if (tick_state.state != EngineTickState::UPDATE_DONE)
            return ObserverReturnSignal::CONTINUE;

        internal_event_dispatcher->queue_event(
            std::make_shared<Event>(engine_events::PRESENT));
    }

    if (tick_state.state == EngineTickState::START &&
        tick_state.object_update_done && tick_state.render_update_done) {
        internal_event_dispatcher->queue_event(
            std::make_shared<Event>(engine_events::RENDER));
    }

    return ObserverReturnSignal::CONTINUE;
}

static bool is_engine_event(const Event &ev) {
    return ev.name.starts_with("engine.");
}

void Engine::subscribe_dispatcher(EventDispatcher &disp) {
    internal_event_dispatcher->register_observer(
        "engine.engine", {engine_events::TICK, engine_events::RENDER_DONE}, 0,
        std::size_t(PipelinePriority::ENGINE), weak_from_this());
}

void Engine::reset_tick_state() { tick_state = {}; }

} // namespace plane_quest::engine