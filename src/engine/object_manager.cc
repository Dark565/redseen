#include "object_manager.hh"

#include <string_view>

namespace plane_quest::engine {

ObjectManager::ObjectManager(Engine &engine) : engine(engine) {}

bool ObjectManager::add_object(const std::string_view &view,
                               const SharedObjectPtr &obj_ptr) {
    return obj_map.insert(std::make_pair(view, obj_ptr)).second;
}
} // namespace plane_quest::engine