#include "event_producer_container.hh"
#include "event_dispatcher.hh"

namespace plane_quest::engine {

std::size_t EventProducerContainer::feed_dispatcher(EventDispatcher &dispatcher,
                                                    bool can_block) {
    std::size_t n_fed = 0;
    for (auto &producer : producers) {
        n_fed += producer.second->feed_dispatcher(dispatcher, can_block);
    }
    return n_fed;
}

bool EventProducerContainer::add_producer(
    const std::string_view &name, std::shared_ptr<EventProducer> producer) {
    return producers.insert(std::make_pair(name, std::move(producer))).second;
}

bool EventProducerContainer::remove_producer(const std::string_view &name) {
    return producers.erase(std::string(name));
}

} // namespace plane_quest::engine