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

#include <concepts>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include "engine/event.hh"
#include "event_dispatcher.hh"
#include "event_observer.hh"

namespace plane_quest::engine {

class Engine;
class Object;

/** Class encapsulating object creation */
class ObjectManager : public EventObserver {
    using SharedObjectPtr = std::shared_ptr<Object>;
    using ObjectMap = std::unordered_map<std::string, SharedObjectPtr>;

    ObjectMap obj_map;
    std::shared_ptr<Engine> engine;

  public:
    ObjectManager(const std::shared_ptr<Engine> &engine);

    template <std::derived_from<Object> T, class... Args>
    std::shared_ptr<T> create_object(const std::string_view &key,
                                     const Args &...args) {
        auto shared_obj = std::make_shared<T>(args...);

        if (!add_object(key, shared_obj))
            throw ObjectManagerException(
                "Object assigned to the specified key already exists");

        return shared_obj;
    }

    /** Get begin and end iterators for objects */
    std::pair<ObjectMap::const_iterator, ObjectMap::const_iterator>
    get_objects() const;

    class ObjectManagerException : public std::logic_error {
      public:
        ObjectManagerException(const char *what) : std::logic_error(what) {}
    };

    ObserverReturnSignal on_event(const Event &event) override;

    static void subscribe_dispatcher(std::weak_ptr<ObjectManager> _this,
                                     EventDispatcher &disp);

  protected:
    void update();

  private:
    bool add_object(const std::string_view &key,
                    const SharedObjectPtr &shared_ptr);

    bool remove_object(const std::string_view &key);

    friend class Engine;
};

} // namespace plane_quest::engine