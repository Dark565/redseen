#include "object_manager.hh"
#include "object/object.hh"
#include "engine/event_observer.hh"

#include <string_view>

namespace plane_quest::engine {

ObjectManager::ObjectManager(const std::shared_ptr<Engine> &engine)
    : engine(engine) {}

bool ObjectManager::add_object(const std::string_view &view,
                               const SharedObjectPtr &obj_ptr) {
    return obj_map.insert(std::make_pair(view, obj_ptr)).second;
}

ObserverReturnSignal ObjectManager::on_event(const Event &event) {
    if (event.type != EventType::TICK)
        return ObserverReturnSignal::CONTINUE;

    for (auto &object_pair : obj_map) {
        object_pair.second->update(engine);
    }
}

} // namespace plane_quest::engine