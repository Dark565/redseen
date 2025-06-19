#include "engine.hh"

#include <memory>
#include <glad/glad.h>

#include "camera.hh"
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

const std::shared_ptr<EventLoop> &Engine::get_event_loop() const {
    return event_loop;
}

void Engine::set_event_loop(const std::shared_ptr<EventLoop> &loop) {
    this->event_loop = loop;
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
    object_manager = std::make_shared<ObjectManager>(shared_from_this());
    texture_manager = std::make_shared<TextureManager>();
}

bool Engine::run() {
    bool ret;

    get_renderer()->init();

    // TODO: Throw an exception if event_loop is not defined

    do {
        event_loop->register_observer(
            "engine.object_manager", 0,
            std::size_t(PipelinePriority::OBJECT_MANAGER),
            get_object_manager());

        event_loop->register_observer("engine.renderer", 0,
                                      std::size_t(PipelinePriority::RENDER),
                                      get_renderer());

        ret = event_loop->run();
    } while (ret); // if return from run() is true that means the event_loop
                   // should be restarted.

    return true;
}

std::shared_ptr<Engine> Engine::create() {
    struct SharedHelper : public Engine {};
    std::shared_ptr<Engine> engine_ = std::make_shared<SharedHelper>();
    engine_->init();
    return engine_;
}

} // namespace plane_quest::engine