#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

#include "object/object.hh"
#include "engine.hh"

namespace plane_quest::engine {

class Engine;
class Object;
class Editor {
    std::shared_ptr<Engine> engine;
    std::unordered_set<std::shared_ptr<Object>> objects;

  public:
    Editor(std::shared_ptr<Engine> engine);

    Editor(Editor &&editor) = default;
    Editor &operator=(Editor &&engine) = default;
};
} // namespace plane_quest::engine