#pragma once

#include <string_view>
#include <memory>

#include "common/noncopyable.hh"
#include "render/mesh_renderer.hh"
#include "texture_manager.hh"
#include "object_manager.hh"
#include "event_loop.hh"
#include "renderer.hh"
#include "camera.hh"

namespace plane_quest::engine {

enum class PipelinePriority : std::size_t { OBJECT_MANAGER = 0, RENDER = 1 };
namespace engine_events {
constexpr std::string_view TICK = "engine.tick";
constexpr std::string_view RENDER = "engine.render";
}; // namespace engine_events

class EventObserver;
class Editor;
class Engine : NonCopyable, public std::enable_shared_from_this<Engine> {
    std::shared_ptr<TextureManager> texture_manager;
    std::shared_ptr<ObjectManager> object_manager;
    std::shared_ptr<EventLoop> event_loop;
    std::shared_ptr<Renderer> renderer;
    Camera player_camera;

    Engine() = default;

    void init();
    void init_opengl();

  public:
    static std::shared_ptr<Engine> create();

    bool run();

    const std::shared_ptr<TextureManager> &get_texture_manager() const;
    const std::shared_ptr<ObjectManager> &get_object_manager() const;
    const std::shared_ptr<Renderer> &get_renderer() const;
    const std::shared_ptr<EventLoop> &get_event_loop() const;
    void set_event_loop(const std::shared_ptr<EventLoop> &);
    void set_renderer(const std::shared_ptr<Renderer> &);

    Camera &get_player_camera();
    const Camera &get_player_camera() const;

    static bool is_engine_event(const Event &);
};

} // namespace plane_quest::engine