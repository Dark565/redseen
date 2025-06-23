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

#include "object_manager.hh"

#ifdef DEBUG
#include <iostream>
#endif

#include "engine/event_dispatcher.hh"
#include "object/object.hh"
#include "engine/event_observer.hh"
#include "engine/engine.hh"

#include <string_view>

namespace redseen::engine {

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
#if 0
    disp.register_observer("engine.object_manager", engine_events::UPDATE, 0,
                           std::size_t(PipelinePriority::OBJECT_MANAGER),
                           _this);
#endif
}

} // namespace redseen::engine