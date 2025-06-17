#pragma once
namespace plane_quest::engine {

struct Event;

enum class ObserverReturnSignal {
    CONTINUE,
    BREAK,
    END_EVENT_LOOP,
    RESTART_EVENT_LOOP
};

class EventObserver {
  public:
    virtual ObserverReturnSignal on_event(const Event &) = 0;
};

} // namespace plane_quest::engine