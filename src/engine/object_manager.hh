#pragma once

#include <concepts>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include "engine/event.hh"
#include "event_observer.hh"

namespace plane_quest::engine {

class Engine;
class Object;

/** Class encapsulating object creation */
class ObjectManager : public EventObserver {
    using SharedObjectPtr = std::shared_ptr<Object>;

    std::unordered_map<std::string, SharedObjectPtr> obj_map;
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

    class ObjectManagerException : public std::logic_error {
      public:
        ObjectManagerException(const char *what) : std::logic_error(what) {}
    };

    /** For EventObserver. Receives TICK events and calls .update() for all
     * objects. */
    ObserverReturnSignal on_event(const Event &event) override;

  private:
    bool add_object(const std::string_view &key,
                    const SharedObjectPtr &shared_ptr);
};

} // namespace plane_quest::engine