#include "event_loop.hh"
#include "engine/event_observer.hh"

namespace plane_quest::engine {

class Event;
class Engine;

bool EventLoop::register_observer(const std::string_view &observer_name,
                                  std::size_t prio,
                                  const std::weak_ptr<EventObserver> &observer,
                                  bool allow_duplicates) {
    if (allow_duplicates &&
        find_observer(observer_name) != event_observer_map.cend())
        return false;

    event_observer_map.emplace(prio, observer_name, observer);
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

bool EventLoop::pass_event(const Event &ev) {
    for (auto iter = event_observer_map.cbegin();
         iter != event_observer_map.cend(); iter++) {
        if (notify_or_remove_observer(iter, ev))
            return true;
    }
    return false;
}

bool EventLoop::notify_or_remove_observer(
    ObserverPrioSet::const_iterator observer_iter, const Event &ev) {
    auto shared_p = observer_iter->observer.lock();
    if (shared_p != nullptr) {
        return shared_p->on_event(ev);
    } else {
        // weak pointer to an observer is expired so remove it
        // from the observer set.
        event_observer_map.erase(observer_iter);
        return false;
    }
}

} // namespace plane_quest::engine