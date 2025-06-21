#pragma once

#include <chrono>

namespace plane_quest::engine {

class EventDispatcher;

/** EventProducers are a direct feed of events for an event dispatcher */
class EventProducer {
  public:
    virtual std::size_t feed_dispatcher(EventDispatcher &) = 0;
};

} // namespace plane_quest::engine