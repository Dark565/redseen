#pragma once
namespace plane_quest::engine {

struct Event;

enum class ObserverReturnSignal {
    CONTINUE,
    DROP_EVENT,
};

class EventObserver {
  public:
    virtual ObserverReturnSignal on_event(const Event &) = 0;
};

} // namespace plane_quest::engine