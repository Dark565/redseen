#pragma once

#include <cstdlib>
#include <glm/glm.hpp>

namespace plane_quest::engine {

enum class EventType {
    /** External event */
    EXTERNAL = 0x1,
    TICK = 0x2,
};

/** Base class for events */
struct Event {
    EventType type;

  protected:
    Event() = default;
};

struct EngineEvent : Event {};

} // namespace plane_quest::engine