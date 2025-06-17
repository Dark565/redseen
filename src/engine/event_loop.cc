#include <limits>
#include "event_loop.hh"
#include "engine/event_observer.hh"

namespace plane_quest::engine {

class Event;
class Engine;

bool EventLoop::register_observer(const std::string_view &observer_name,
                                  std::size_t priority_class,
                                  std::size_t priority,
                                  const std::weak_ptr<EventObserver> &observer,
                                  bool allow_duplicates) {
    if (allow_duplicates &&
        find_observer(observer_name) != event_observer_map.cend())
        return false;

    event_observer_map.emplace(wrap_prio(priority_class, priority),
                               observer_name, observer);
    return true;
}

bool EventLoop::unregister_observer(const std::string_view &observer_name) {
    auto observer_iter = find_observer(observer_name);
    if (observer_iter != event_observer_map.cend()) {
        event_observer_map.erase(observer_iter);
        return true;
    } else {
        return false;
    }
}

bool EventLoop::set_observer_priority(const std::string_view &observer_name,
                                      std::size_t prio,
                                      bool include_duplicates) {
    auto set_prio_for_one = [&](ObserverPrioSet::const_iterator iter) {
        auto old_key = *iter;
        event_observer_map.erase(iter);
        event_observer_map.insert(PriorityObserverKey{
            .prio = prio, .name = old_key.name, .observer = old_key.observer});
    };

    bool any_removed = false;
    ObserverPrioSet::const_iterator iter;

    while ((iter = find_observer(observer_name)) != event_observer_map.cend()) {
        any_removed = true;
        set_prio_for_one(iter);
        if (!include_duplicates)
            break;
    }

    return any_removed;
}

EventLoop::ObserverPrioSet::const_iterator
EventLoop::find_observer(const std::string_view &observer_name) {
    auto iter = event_observer_map.cbegin();
    for (; iter != event_observer_map.cend(); iter++) {
        if (iter->name == observer_name)
            return iter;
    }
    return iter;
}

EventLoopStatusPair EventLoop::pass_event(const Event &ev) {
    EventLoopStatusPair observer_res = {false, ObserverReturnSignal::CONTINUE};

    bool any_processed = false;

    for (auto iter = event_observer_map.cbegin();
         iter != event_observer_map.cend(); iter++) {
        observer_res = notify_or_remove_observer(iter, ev);
        if (observer_res.first)
            any_processed = true;

        if (observer_res.first &&
                observer_res.second == ObserverReturnSignal::BREAK ||
            observer_res.second == ObserverReturnSignal::END_EVENT_LOOP)
            return observer_res;
    }

    return {any_processed, observer_res.second};
}

EventLoopStatusPair EventLoop::notify_or_remove_observer(
    ObserverPrioSet::const_iterator observer_iter, const Event &ev) {
    auto shared_p = observer_iter->observer.lock();
    if (shared_p != nullptr) {
        return {true, shared_p->on_event(ev)};
    } else {
        // weak pointer to an observer is expired so remove it
        // from the observer set.
        event_observer_map.erase(observer_iter);
        return {false, ObserverReturnSignal::CONTINUE};
    }
}

bool EventLoop::should_be_closed(const EventLoopStatusPair &el_status) {
    return (el_status.first == false ||
            el_status.second == ObserverReturnSignal::END_EVENT_LOOP);
}

bool EventLoop::return_status(const EventLoopStatusPair &el_status) {
    /** If event loop should be restarted, true is returned. */
    return el_status.second == ObserverReturnSignal::RESTART_EVENT_LOOP;
}

std::size_t EventLoop::wrap_prio(std::size_t prefix, std::size_t prio) {
    constexpr auto sizet_bits = std::numeric_limits<std::size_t>::digits;
    constexpr auto shift_bits = sizet_bits / 2;
    constexpr auto low_mask = (std::size_t(1) << shift_bits) - 1;
    auto clamped_prefix = std::min(prefix, low_mask);
    auto clamped_prio = std::min(prio, low_mask);

    return ((clamped_prefix & low_mask) << shift_bits) |
           (clamped_prio & low_mask);
}

std::pair<std::size_t, std::size_t> EventLoop::unwrap_prio(std::size_t wprio) {
    constexpr auto sizet_bits = std::numeric_limits<std::size_t>::digits;
    constexpr auto shift_bits = sizet_bits / 2;
    constexpr auto low_mask = (std::size_t(1) << shift_bits) - 1;

    return {wprio >> shift_bits, wprio & low_mask};
}

} // namespace plane_quest::engine