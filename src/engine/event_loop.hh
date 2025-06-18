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

template <> struct std::less<plane_quest::engine::PriorityObserverKey> {
    bool operator()(const plane_quest::engine::PriorityObserverKey &a,
                    const plane_quest::engine::PriorityObserverKey &b) const {
        return std::less<decltype(a.prio)>{}(a.prio, b.prio);
    }
};

namespace plane_quest::engine {

using EventLoopStatusPair = std::pair<bool, ObserverReturnSignal>;

/** The base class for priority event dispatchers.
Lower priority value = higher priority.
User observers should have priority greater than 0 as 0 is reserved
for the engine */
class EventLoop {
    using ObserverPrioSet = std::multiset<PriorityObserverKey>;

    ObserverPrioSet event_observer_map;

  public:
    bool register_observer(const std::string_view &observer_name,
                           std::size_t priority_class, std::size_t priority,
                           const std::weak_ptr<EventObserver> &,
                           bool allow_duplicates = false);

    bool unregister_observer(const std::string_view &observer_name);

    bool set_observer_priority(const std::string_view &observer_name,
                               std::size_t prio,
                               bool include_duplicates = false);

    virtual bool run() = 0;

    /** Wrap priority to be used as priority for registering observers.
    Engine's internal prefix is 0. */
    static std::size_t wrap_prio(size_t prefix, size_t prio);

    /** Unwrap priority used for registering. */
    static std::pair<std::size_t, std::size_t>
    unwrap_prio(std::size_t wrapped_prio);

  protected:
    ObserverPrioSet::const_iterator
    find_observer(const std::string_view &observer_name);

    EventLoopStatusPair
    notify_or_remove_observer(ObserverPrioSet::const_iterator observer_iter,
                              const Event &ev);

    EventLoopStatusPair pass_event(const Event &ev);

    /** Check if the event loop should be closed based on a status. */
    bool should_be_closed(const EventLoopStatusPair &status);

    /** Get a status that an EventLoop should return on exit fron run(). */
    bool return_status(const EventLoopStatusPair &status);

    EventLoop() = default;
    friend class Engine;
};

} // namespace plane_quest::engine