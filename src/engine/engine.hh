#pragma once

#include <memory>

#include "common/noncopyable.hh"
#include "render/mesh_renderer.hh"
#include "texture_manager.hh"
#include "object_manager.hh"
#include "event_loop.hh"

namespace plane_quest::engine {

class EventObserver;
class Editor;
class Engine : NonCopyable, public std::enable_shared_from_this<Engine> {
    TextureManager texture_manager;
    ObjectManager object_manager;
    std::shared_ptr<EventLoop> event_loop;
    render::MeshRenderer global_mesh_renderer;

    Engine();

  public:
    static std::shared_ptr<Engine> create();

    bool run();

    std::shared_ptr<Editor> new_editor();
    TextureManager &get_texture_manager();
    const TextureManager &get_texture_manager() const;

    ObjectManager &get_object_manager();
    const ObjectManager &get_object_manager() const;

    const std::shared_ptr<EventLoop> &get_event_loop() const;
    void set_event_loop(const std::shared_ptr<EventLoop> &);

    render::MeshRenderer &get_global_mesh_renderer();
    const render::MeshRenderer &get_global_mesh_renderer() const;
};

} // namespace plane_quest::engine