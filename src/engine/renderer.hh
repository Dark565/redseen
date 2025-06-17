#pragma once

#include "event_observer.hh"

namespace plane_quest::engine {

class Engine;
class Event;

class Renderer : public EventObserver {
  public:
    ObserverReturnSignal on_event(const Event &ev) override;
};

} // namespace plane_quest::engine