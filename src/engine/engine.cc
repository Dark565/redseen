#include "engine.hh"
#include "camera.hh"
#include "engine/texture_manager.hh"
#include "event_observer.hh"
#include "render/mesh_renderer.hh"

#include <memory>

namespace plane_quest::engine {

class EventLoop;

Engine::Engine()
    : object_manager(std::make_shared<ObjectManager>(shared_from_this())),
      texture_manager(std::make_shared<TextureManager>()) {}

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

render::MeshRenderer &Engine::get_global_mesh_renderer() {
    return global_mesh_renderer;
}

const render::MeshRenderer &Engine::get_global_mesh_renderer() const {
    return global_mesh_renderer;
}

Camera &Engine::get_player_camera() { return player_camera; }

const Camera &Engine::get_player_camera() const { return player_camera; }

bool Engine::run() {
    bool ret;

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

} // namespace plane_quest::engine