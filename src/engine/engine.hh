#pragma once

#include <memory>

#include "common/noncopyable.hh"
#include "engine/event_observer.hh"
#include "render/mesh_renderer.hh"
#include "texture_manager.hh"
#include "object_manager.hh"
#include "event_loop.hh"
#include "event_observer.hh"
#include "renderer.hh"
#include "camera.hh"

namespace plane_quest::engine {

enum class PipelinePriority : std::size_t { OBJECT_MANAGER = 0, RENDER = 1 };

class EventObserver;
class Editor;
class Engine : NonCopyable, public std::enable_shared_from_this<Engine> {
    std::shared_ptr<TextureManager> texture_manager;
    std::shared_ptr<ObjectManager> object_manager;
    std::shared_ptr<EventLoop> event_loop;
    std::shared_ptr<Renderer> renderer;
    render::MeshRenderer global_mesh_renderer;
    Camera player_camera;

    Engine();

  public:
    static std::shared_ptr<Engine> create();

    bool run();

    std::shared_ptr<Editor> new_editor();
    const std::shared_ptr<TextureManager> &get_texture_manager() const;
    const std::shared_ptr<ObjectManager> &get_object_manager() const;
    const std::shared_ptr<Renderer> &get_renderer() const;
    const std::shared_ptr<EventLoop> &get_event_loop() const;
    void set_event_loop(const std::shared_ptr<EventLoop> &);

    render::MeshRenderer &get_global_mesh_renderer();
    const render::MeshRenderer &get_global_mesh_renderer() const;

    Camera &get_player_camera();
    const Camera &get_player_camera() const;
};

} // namespace plane_quest::engine