#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include "engine/event_dispatcher.hh"
#include "event_producer_container.hh"
#include "event_producer.hh"

namespace plane_quest::engine {
/** A class for event producers */
class EventProducerContainer {
    std::unordered_map<std::string, std::shared_ptr<EventProducer>> producers;

  public:
    std::size_t feed_dispatcher(EventDispatcher &, bool can_block = false);

    bool add_producer(const std::string_view &name,
                      std::shared_ptr<EventProducer> producer);
    bool remove_producer(const std::string_view &name);
};

} // namespace plane_quest::engine