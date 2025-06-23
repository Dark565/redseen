#pragma once

#include <memory>
#include <unordered_set>

#include "engine/event_observer.hh"

namespace plane_quest::engine {
class Engine;
class Object;
}; // namespace plane_quest::engine

namespace plane_quest::demos::particles {
class Editor : engine::EventObserver {
    std::shared_ptr<engine::Engine> engine;
    std::unordered_set<std::shared_ptr<engine::Object>> objects;

    Editor(std::shared_ptr<engine::Engine> engine);

  public:
    Editor(Editor &&editor) = default;
    Editor &operator=(Editor &&engine) = default;

    friend class Engine;
};
} // namespace plane_quest::demos::particles