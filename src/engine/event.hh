#pragma once

#include <cstdlib>

namespace plane_quest::engine {

enum class EventType : std::uint64_t {
    /** Single tick of the engine */
    TICK = 0x1
};

struct Event {
    EventType type;
};

} // namespace plane_quest::engine