#include "object_manager.hh"

#ifdef DEBUG
#include <iostream>
#endif

#include "engine/event_dispatcher.hh"
#include "object/object.hh"
#include "engine/event_observer.hh"
#include "engine/engine.hh"

#include <string_view>

namespace plane_quest::engine {

ObjectManager::ObjectManager(const std::shared_ptr<Engine> &engine)
    : engine(engine) {}

bool ObjectManager::add_object(const std::string_view &view,
                               const SharedObjectPtr &obj_ptr) {
    return obj_map.insert(std::make_pair(view, obj_ptr)).second;
}

bool ObjectManager::remove_object(const std::string_view &view) {
    return obj_map.erase(std::string{view});
}

ObserverReturnSignal ObjectManager::on_event(const Event &event) {
    if (!event.has_name(engine_events::UPDATE))
        return ObserverReturnSignal::CONTINUE;

    update();

    engine->get_internal_event_dispatcher()->queue_next(
        std::make_shared<Event>(engine_events::OBJECT_UPDATE_DONE));

    return ObserverReturnSignal::CONTINUE;
}

std::pair<ObjectManager::ObjectMap::const_iterator,
          ObjectManager::ObjectMap::const_iterator>
ObjectManager::get_objects() const {
    return std::make_pair(obj_map.cbegin(), obj_map.cend());
}

void ObjectManager::update() {
#ifdef DEBUG
    std::cerr << "ObjetManager: update()" << std::endl;
#endif
    for (const auto &object_pair : obj_map) {
        ObjectUpdateResult result = object_pair.second->update(*engine);
        switch (result) {
        case ObjectUpdateResult::DESTROY:
            remove_object(object_pair.first);
            break;
        default:;
        }
    }
}

void ObjectManager::subscribe_dispatcher(std::weak_ptr<ObjectManager> _this,
                                         EventDispatcher &disp) {
    disp.register_observer("engine.object_manager", engine_events::UPDATE, 0,
                           std::size_t(PipelinePriority::OBJECT_MANAGER),
                           _this);
}

} // namespace plane_quest::engine