#pragma once
namespace plane_quest::engine {

struct Event;

class EventObserver {
  public:
    virtual bool on_event(const Event &) = 0;
};

} // namespace plane_quest::engine