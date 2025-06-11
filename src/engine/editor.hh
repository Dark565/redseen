#pragma once

#include <memory>
#include <unordered_set>

#include "object/object.hh"
#include "event_observer.hh"
#include "engine.hh"

namespace plane_quest::engine {

class Engine;
class Object;
class Editor : EventObserver {
    std::shared_ptr<Engine> engine;
    std::unordered_set<std::shared_ptr<Object>> objects;

    Editor(std::shared_ptr<Engine> engine);

  public:
    Editor(Editor &&editor) = default;
    Editor &operator=(Editor &&engine) = default;

    friend class Engine;
};
} // namespace plane_quest::engine