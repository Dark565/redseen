#pragma once

#include <vector>
#include <memory>

#include "common/noncopyable.hh"
#include "render/mesh_renderer.hh"
#include "texture_manager.hh"
#include "object_manager.hh"

namespace plane_quest::engine {

class EventObserver;
class Editor;
class Engine : NonCopyable, public std::enable_shared_from_this<Engine> {
    TextureManager texture_manager;
    ObjectManager object_manager;
    render::MeshRenderer global_mesh_renderer;
    std::vector<std::weak_ptr<EventObserver>> event_observers;

    Engine();

  public:
    static std::shared_ptr<Engine> create();

    std::unique_ptr<Editor> new_editor();
    TextureManager &get_texture_manager();
    const TextureManager &get_texture_manager() const;

    ObjectManager &get_object_manager();
    const ObjectManager &get_object_manager() const;

    render::MeshRenderer &get_global_mesh_renderer();
    const render::MeshRenderer &get_global_mesh_renderer() const;
};

} // namespace plane_quest::engine