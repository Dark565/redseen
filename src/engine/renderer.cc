#include "renderer.hh"

#include "event.hh"
#include "engine/engine.hh"
#include "engine/event_observer.hh"
#include "model.hh"

namespace plane_quest::engine {

Renderer::Renderer(std::shared_ptr<Engine> engine) : engine(engine) {}

bool Renderer::render(const RenderRequest &req) {
    return req.model.render(*this, req,
                            engine->get_player_camera().getViewMatrix());
}

void Renderer::subscribe_dispatcher(std::weak_ptr<Renderer> _this,
                                    EventDispatcher &disp) {
    disp.register_observer(
        "engine.renderer",
        {engine_events::UPDATE, engine_events::RENDER, engine_events::PRESENT},
        0, std::size_t(PipelinePriority::RENDER), _this);
}

ObserverReturnSignal Renderer::on_event(const Event &ev) {
    auto &int_disp = *engine->get_internal_event_dispatcher();
    if (ev.has_name(engine_events::UPDATE)) {
        update();
        int_disp.queue_last(
            std::make_unique<Event>(engine_events::RENDER_UPDATE_DONE));
    } else if (ev.has_name(engine_events::RENDER)) {
        render();
        int_disp.queue_last(
            std::make_unique<Event>(engine_events::RENDER_DONE));
    } else if (ev.has_name(engine_events::PRESENT)) {
        present();
    }
    return ObserverReturnSignal::CONTINUE;
}

void Renderer::render() {
    auto &om = *engine->get_object_manager();
    auto [obj_beg, obj_end] = om.get_objects();

    for (auto iter = obj_beg; iter != obj_end; iter++) {
        // TODO: do render of obj
    }
}

} // namespace plane_quest::engine