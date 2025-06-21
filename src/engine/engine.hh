#pragma once

#include <string_view>
#include <memory>

#include "common/noncopyable.hh"
#include "engine/event_observer.hh"
#include "render/mesh_renderer.hh"
#include "texture_manager.hh"
#include "object_manager.hh"
#include "event_dispatcher.hh"
#include "renderer.hh"
#include "camera.hh"

namespace plane_quest::engine {

enum class PipelinePriority : std::size_t {
    ENGINE = 0,
    OBJECT_MANAGER = 1,
    RENDER = 2
};
namespace engine_events {
constexpr std::string_view TICK = "engine.tick";
constexpr std::string_view UPDATE = "engine.update";
constexpr std::string_view OBJECT_UPDATE_DONE = "engine.om_update.done";
constexpr std::string_view RENDER_UPDATE_DONE = "engine.render_update.done";
constexpr std::string_view RENDER = "engine.render";
constexpr std::string_view PRESENT = "engine.present";
constexpr std::string_view RENDER_DONE = "engine.render.done";

}; // namespace engine_events

struct EngineTickState {
    enum {
        START = 0,
        UPDATE_DONE = 1,
        RENDER_DONE = 2,
        PRESENT_DONE = 3
    } state = START;

    bool object_update_done : 1;
    bool render_update_done : 1;
};

class EventObserver;
class Editor;
class Engine : NonCopyable,
               public EventObserver,
               public std::enable_shared_from_this<Engine> {
    std::unique_ptr<EventDispatcher> event_dispatcher;
    std::unique_ptr<EventDispatcher> internal_event_dispatcher;
    std::shared_ptr<TextureManager> texture_manager;
    std::shared_ptr<ObjectManager> object_manager;
    std::shared_ptr<Renderer> renderer;
    Camera player_camera;
    EngineTickState tick_state;

    Engine() = default;

    void init();
    void init_opengl();
    void subscribe_dispatcher(EventDispatcher &);

    void reset_tick_state();

  public:
    static std::shared_ptr<Engine> create();

    bool run();

    const std::unique_ptr<EventDispatcher> &get_event_dispatcher() const;
    const std::unique_ptr<EventDispatcher> &
    get_internal_event_dispatcher() const;
    const std::shared_ptr<TextureManager> &get_texture_manager() const;
    const std::shared_ptr<ObjectManager> &get_object_manager() const;
    const std::shared_ptr<Renderer> &get_renderer() const;
    void set_renderer(const std::shared_ptr<Renderer> &);

    Camera &get_player_camera();
    const Camera &get_player_camera() const;

    static bool is_engine_event(const Event &);

    ObserverReturnSignal on_event(const Event &) override;
};

} // namespace plane_quest::engine