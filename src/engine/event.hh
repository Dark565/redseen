#pragma once

#ifdef DEBUG
#include <iostream>
#endif

#include <cstdlib>
#include <string_view>
#include <glm/glm.hpp>

namespace plane_quest::engine {

/** Base class for events */
struct Event {
    using Key = std::string;
    using KeyView = std::string_view;

    KeyView name;

    Event(const KeyView &name) : name(name) {
#ifdef DEBUG
        std::cerr << "Creating event: " << name << std::endl;
#endif
    }

    bool has_name(const KeyView &name) const { return this->name == name; }

    virtual ~Event() = default;
};

struct EngineEvent : Event {};

} // namespace plane_quest::engine