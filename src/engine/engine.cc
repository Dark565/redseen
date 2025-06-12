#include "engine.hh"

#include <memory>

#include "editor.hh"

namespace plane_quest::engine {

class EventLoop;

Engine::Engine() : object_manager(*this) {}

std::shared_ptr<Editor> Engine::new_editor() {
    return std::make_shared<Editor>(shared_from_this());
}

TextureManager &Engine::get_texture_manager() { return texture_manager; }

const TextureManager &Engine::get_texture_manager() const {
    return texture_manager;
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

} // namespace plane_quest::engine