/*
 *  Copyright (C) 2025 Grzegorz Kociołek (grzegorz.kclk@gmail.com)
 *
 *  This file is a part of RedSeen; a 3D game engine.
 *
 *  RedSeen is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RedSeen is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <chrono>
#include <string_view>
#include <memory>

#include "common/noncopyable.hh"
#include "engine/event_observer.hh"
#include "engine/event_producer_container.hh"
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

enum class EngineFrameState {
    START = 0,
    WAITING_FOR_UPDATE = 1,
    WAITING_FOR_RENDER = 2,
};

class EventObserver;
class Editor;
class Engine : NonCopyable,
               public EventObserver,
               public EventProducer,
               public std::enable_shared_from_this<Engine> {
    /** Event dispatcher for general events such as
     window events, object events */
    std::unique_ptr<EventDispatcher> event_dispatcher;
    /** Event dispatcher for engine events such as the TICK event, the UPDATE
    event. It is fed by the engine components itself, not EventProducers. */
    std::unique_ptr<EventDispatcher> internal_event_dispatcher;
    std::unique_ptr<EventProducerContainer> event_producers;
    std::unique_ptr<EventProducerContainer> internal_event_producers;
    std::shared_ptr<TextureManager> texture_manager;
    std::shared_ptr<ObjectManager> object_manager;
    std::shared_ptr<Renderer> renderer;
    Camera player_camera;
    std::chrono::time_point<std::chrono::steady_clock> tick_start_time;

    class FrameState {
        Engine &engine;
        EngineFrameState state;

      public:
        FrameState(Engine &engine, EngineFrameState state)
            : engine(engine), state(state) {}

        virtual void take_event(const Event &ev) = 0;

        class Start;
        class WaitingForUpdate;
        class WaitingForRender;
    };

    class Start;

    std::unique_ptr<FrameState> frame_state;

    Engine() = default;

    void init();
    void init_opengl();
    void subscribe_dispatcher(EventDispatcher &);

    void reset_frame_state();
    void internal_dispatch_loop();

    void receive_external_events();
    void dispatch_external_events();
    void handle_external_events();

    void handle_frame();

  public:
    static std::shared_ptr<Engine> create();

    bool run();

    const std::unique_ptr<EventDispatcher> &get_event_dispatcher() const;
    const std::unique_ptr<EventDispatcher> &
    get_internal_event_dispatcher() const;

    const std::unique_ptr<EventProducerContainer> &
    get_internal_event_producer_container() const;
    const std::unique_ptr<EventProducerContainer> &
    get_event_producer_container() const;

    const std::shared_ptr<TextureManager> &get_texture_manager() const;
    const std::shared_ptr<ObjectManager> &get_object_manager() const;
    const std::shared_ptr<Renderer> &get_renderer() const;
    void set_renderer(const std::shared_ptr<Renderer> &);

    Camera &get_player_camera();
    const Camera &get_player_camera() const;

    static bool is_engine_event(const Event &);

    ObserverReturnSignal on_event(const Event &) override;

    std::size_t feed_dispatcher(EventDispatcher &, bool can_block) override;
};

} // namespace plane_quest::engine