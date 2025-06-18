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

bool ObjectManager::remove_object(const std::string_view &view) {
    return obj_map.erase(std::string{view});
}

ObserverReturnSignal ObjectManager::on_event(const Event &event) {
    if (event.type != EventType::TICK)
        return ObserverReturnSignal::CONTINUE;

    for (auto &object_pair : obj_map) {
        ObjectUpdateResult result = object_pair.second->update(engine);
        switch (result) {
        case ObjectUpdateResult::DESTROY:
            remove_object(object_pair.first);
            break;
        }
    }
}

} // namespace plane_quest::engine