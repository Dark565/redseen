#include "renderer.hh"

#include "event.hh"
#include "engine/engine.hh"
#include "engine/event_observer.hh"
#include "engine/object/object.hh"
#include "model.hh"

namespace plane_quest::engine {

Renderer::Renderer(std::shared_ptr<Engine> engine) : engine(engine) {}

Renderer::~Renderer() {}

bool Renderer::render(const RenderRequest &req) {
    return req.model.render(*this, req,
                            engine->get_player_camera().getViewMatrix());
}

void Renderer::subscribe_dispatcher(std::weak_ptr<Renderer> _this,
                                    EventDispatcher &disp) {
#if 0
    disp.register_observer(
        "engine.renderer",
        {engine_events::UPDATE, engine_events::RENDER, engine_events::PRESENT},
        0, std::size_t(PipelinePriority::RENDER), _this);
#endif
}

ObserverReturnSignal Renderer::on_event(const Event &ev) {
#if 0
    auto &int_disp = *engine->get_internal_event_dispatcher();
    if (ev.has_name(engine_events::UPDATE)) {
        update();
        int_disp.queue_next(
            std::make_unique<Event>(engine_events::RENDER_UPDATE_DONE));
    } else if (ev.has_name(engine_events::RENDER)) {
        render();
        int_disp.queue_next(
            std::make_unique<Event>(engine_events::RENDER_DONE));
    } else if (ev.has_name(engine_events::PRESENT)) {
        present();
    }
#endif
    return ObserverReturnSignal::CONTINUE;
}

void Renderer::render() {
    auto &om = *engine->get_object_manager();
    auto [obj_beg, obj_end] = om.get_objects();

    for (auto iter = obj_beg; iter != obj_end; iter++) {
#if DEBUG
        std::cerr << "Renderer::render(): rendering '" << iter->first << "'"
                  << std::endl;
#endif
        iter->second->render(*engine);
        // TODO: do render of obj
    }
}

} // namespace plane_quest::engine