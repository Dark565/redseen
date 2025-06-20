#pragma once

#include <cstdlib>
#include <string_view>
#include <glm/glm.hpp>

namespace plane_quest::engine {

/** Base class for events */
struct Event {
    std::string_view name;

    Event(const std::string_view &name) : name(name) {}

    bool has_name(const std::string_view &name) const {
        return this->name == name;
    }

    virtual ~Event() = default;
};

struct EngineEvent : Event {};

} // namespace plane_quest::engine