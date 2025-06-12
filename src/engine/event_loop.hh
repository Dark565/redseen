#pragma once

#include <set>
#include <string>
#include <string_view>
#include <memory>

#include "event_observer.hh"

namespace plane_quest::engine {

class Event;
class Engine;

struct PriorityObserverKey {
    std::size_t prio;
    std::string name;
    std::weak_ptr<EventObserver> observer;
};

} // namespace plane_quest::engine

template <> struct std::greater<plane_quest::engine::PriorityObserverKey> {
    bool operator()(const plane_quest::engine::PriorityObserverKey &a,
                    const plane_quest::engine::PriorityObserverKey &b) {
        return std::greater<std::size_t>{}(a.prio, b.prio);
    }
};

namespace plane_quest::engine {

using EventLoopStatusPair = std::pair<bool, ObserverReturnSignal>;

/** The base class for event dispatchers. */
class EventLoop {
    using ObserverPrioSet =
        std::multiset<PriorityObserverKey, std::greater<PriorityObserverKey>>;

    ObserverPrioSet event_observer_map;

  public:
    bool register_observer(const std::string_view &observer_name,
                           std::size_t prio,
                           const std::weak_ptr<EventObserver> &,
                           bool allow_duplicates = false);

    bool unregister_observer(const std::string_view &observer_name);

    bool set_observer_priority(const std::string_view &observer_name,
                               std::size_t prio,
                               bool include_duplicates = false);

    virtual bool run() = 0;

  protected:
    ObserverPrioSet::const_iterator
    find_observer(const std::string_view &observer_name);

    EventLoopStatusPair
    notify_or_remove_observer(ObserverPrioSet::const_iterator observer_iter,
                              const Event &ev);

    EventLoopStatusPair pass_event(const Event &ev);

    /** Check if the event loop should be closed based on a status. */
    bool should_be_closed(const EventLoopStatusPair &status);

    EventLoop() = default;
    friend class Engine;
};

} // namespace plane_quest::engine