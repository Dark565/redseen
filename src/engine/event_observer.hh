#pragma once

#include "event.hh"

namespace plane_quest::engine {

class EventObserver {
  public:
    virtual void on_event(const Event &) = 0;
};

} // namespace plane_quest::engine