#include "engine.hh"

#include <chrono>
#include <thread>
#include <memory>
#include <glad/glad.h>

#include "camera.hh"
#include "engine/event_dispatcher.hh"
#include "engine/event_observer.hh"
#include "engine/event_producer_container.hh"
#include "engine/object/object.hh"
#include "engine/texture_manager.hh"
namespace plane_quest::engine {

class EventLoop;

#if 0
class Engine::FrameState::Start : public Engine::FrameState {
  public:
    Start(Engine &engine)
        : Engine::FrameState(engine, EngineFrameState::START) {}
    void take_event(const Event &ev) override;
};
class Engine::FrameState::WaitingForUpdate : public Engine::FrameState {
    bool object_update_done : 1 = false;
    bool render_update_done : 1 = false;

  public:
    WaitingForUpdate(Engine &engine)
        : Engine::FrameState(engine, EngineFrameState::WAITING_FOR_UPDATE) {}
    void take_event(const Event &ev) override;
};
class Engine::FrameState::WaitingForRender : public Engine::FrameState {
  public:
    WaitingForRender(Engine &engine)
        : Engine::FrameState(engine, EngineFrameState::WAITING_FOR_RENDER) {}
    void take_event(const Event &ev) override;
};

void Engine::FrameState::Start::take_event(const Event &ev) {
    if (!ev.has_name(engine_events::TICK))
        return;

    engine.get_internal_event_dispatcher()->queue_next(
        std::make_shared<Event>(engine_events::UPDATE));

    engine.frame_state = std::unique_ptr<FrameState>(
        new Engine::FrameState::WaitingForUpdate(engine));
}

void Engine::FrameState::WaitingForUpdate::take_event(const Event &ev) {
    if (ev.has_name(engine_events::OBJECT_UPDATE_DONE))
        object_update_done = true;
    else if (ev.has_name(engine_events::RENDER_UPDATE_DONE))
        render_update_done = true;
    else
        return;

    if (object_update_done && render_update_done) {
        engine.get_internal_event_dispatcher()->queue_next(
            std::make_shared<Event>(engine_events::RENDER));
        engine.frame_state = std::unique_ptr<FrameState>(
            new Engine::FrameState::WaitingForRender(engine));
    }
}

void Engine::FrameState::WaitingForRender::take_event(const Event &ev) {
    if (!ev.has_name(engine_events::RENDER_DONE))
        return;

    engine.get_internal_event_dispatcher()->queue_next(
        std::make_shared<Event>(engine_events::PRESENT));

    engine.frame_state =
        std::unique_ptr<FrameState>(new Engine::FrameState::Start(engine));
}

#endif

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

const std::unique_ptr<EventProducerContainer> &
Engine::get_event_producer_container() const {
    return event_producers;
}

const std::unique_ptr<EventProducerContainer> &
Engine::get_internal_event_producer_container() const {
    return internal_event_producers;
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
    event_producers = std::make_unique<EventProducerContainer>();
    internal_event_producers = std::make_unique<EventProducerContainer>();
    object_manager = std::make_shared<ObjectManager>(shared_from_this());
    texture_manager = std::make_shared<TextureManager>();
}

bool Engine::run() {
    bool ret;

    get_renderer()->init();
    reset_frame_state();

    subscribe_dispatcher(*internal_event_dispatcher);
    get_renderer()->subscribe_dispatcher(get_renderer(),
                                         *internal_event_dispatcher);

    get_object_manager()->subscribe_dispatcher(get_object_manager(),
                                               *internal_event_dispatcher);

    internal_event_producers->add_producer("engine", shared_from_this());

    tick_start_time = std::chrono::steady_clock::now();

    internal_dispatch_loop();
    return true;
}

std::shared_ptr<Engine> Engine::create() {
    struct SharedHelper : public Engine {};
    std::shared_ptr<Engine> engine_ = std::make_shared<SharedHelper>();
    engine_->init();
    return engine_;
}

ObserverReturnSignal Engine::on_event(const Event &ev) {
#if 0
    if (ev.has_name(engine_events::UPDATE)) {
        handle_external_events();
    }

    frame_state->take_event(ev);
#endif

    if (!ev.has_name(engine_events::TICK))
        return ObserverReturnSignal::CONTINUE;

    handle_frame();
    return ObserverReturnSignal::CONTINUE;
}

void Engine::handle_frame() {
#ifdef DEBUG
    std::cerr << "---- Engine::handle_frame(): called ---" << std::endl;
#endif

    object_manager->update();
    renderer->update();
    handle_external_events();
    renderer->render();
    renderer->present();
}

static bool is_engine_event(const Event &ev) {
    return ev.name.starts_with("engine.");
}

void Engine::subscribe_dispatcher(EventDispatcher &disp) {
    internal_event_dispatcher->register_observer(
        "engine.engine",
        {engine_events::TICK, engine_events::OBJECT_UPDATE_DONE,
         engine_events::RENDER_UPDATE_DONE, engine_events::RENDER_DONE,
         engine_events::UPDATE},
        0, std::size_t(PipelinePriority::ENGINE), weak_from_this());
}

void Engine::reset_frame_state() {
#if 0
    frame_state = std::make_unique<Engine::FrameState::Start>(*this);
#endif
}

void Engine::internal_dispatch_loop() {
    while (1) {
        internal_event_producers->feed_dispatcher(*internal_event_dispatcher,
                                                  true);

        internal_event_dispatcher->dispatch();
    }
}

void Engine::receive_external_events() {
    event_producers->feed_dispatcher(*event_dispatcher, false);
}

void Engine::dispatch_external_events() { event_dispatcher->dispatch(); }

void Engine::handle_external_events() {
    receive_external_events();
    dispatch_external_events();
}

/** Feeds engine with TICK each 16ms (todo: make it configurable, dependent on
 * vsync etc) */
std::size_t Engine::feed_dispatcher(EventDispatcher &disp, bool can_block) {
    constexpr auto TICK_DELAY = std::chrono::milliseconds(16);
    constexpr std::size_t MAX_CONSECUTIVE_TICKS = 32;

    auto cur_time = std::chrono::steady_clock::now();
    auto time_delta = cur_time - tick_start_time;

    if (time_delta >= TICK_DELAY) {
        std::size_t n_ticks = std::min(std::size_t(time_delta / TICK_DELAY),
                                       MAX_CONSECUTIVE_TICKS);
        auto event = std::make_shared<Event>(engine_events::TICK);
        for (auto i = 0; i < n_ticks; i++)
            disp.queue_last(event);

        tick_start_time = cur_time;
        return n_ticks;
    } else if (can_block) {
        std::this_thread::sleep_for(TICK_DELAY - time_delta);
        disp.queue_last(std::make_shared<Event>(engine_events::TICK));
        tick_start_time += TICK_DELAY;
        return 1;
    } else {
        return 0;
    }
}

} // namespace plane_quest::engine