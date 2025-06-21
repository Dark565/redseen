#include <limits>
#include <optional>
#include <string>
#include "event_dispatcher.hh"
#include "engine/event_observer.hh"

namespace plane_quest::engine {

class Event;
class Engine;

bool EventDispatcher::register_observer(
    const std::string_view &observer_name, const Event::KeyView &event_name,
    std::size_t priority_class, std::size_t priority,
    const std::weak_ptr<EventObserver> &observer, bool allow_duplicates) {
    if (allow_duplicates &&
        find_observer(observer_name, event_name).has_value())
        return false;

    get_priority_set(event_name)
        .emplace(wrap_prio(priority_class, priority),
                 std::string(observer_name), observer);

    return true;
}

bool EventDispatcher::unregister_observer(const std::string_view &observer_name,
                                          const Event::KeyView &event_name) {
    if (auto query = find_observer(observer_name, event_name);
        query.has_value()) {
        query->first.erase(query->second);
        if (query->first.empty())
            event_observer_map.erase(std::string(event_name));
        return true;
    } else {
        return false;
    }
}

bool EventDispatcher::set_observer_priority(
    const std::string_view &observer_name, const Event::KeyView &event_name,
    std::size_t prio, bool include_duplicates) {

    auto set_prio_for_one = [&](const ObserverQuery &query) {
        auto old_key = *query.second;
        query.first.erase(query.second);
        query.first.insert(PriorityObserverKey{
            .prio = prio, .name = old_key.name, .observer = old_key.observer});
    };

    bool any_removed = false;

    std::optional<ObserverQuery> query;
    while ((query = find_observer(observer_name, event_name)).has_value()) {
        any_removed = true;
        set_prio_for_one(*query);
        if (!include_duplicates)
            break;
    }

    return any_removed;
}

void EventDispatcher::queue_last(std::shared_ptr<Event> ev) {
    event_queue.emplace_back(std::move(ev));
}

void EventDispatcher::queue_next(std::shared_ptr<Event> ev) {
    event_queue.emplace_front(std::move(ev));
}

void EventDispatcher::drop_queue() { event_queue = {}; }

std::size_t EventDispatcher::dispatch(std::size_t n) {
    std::size_t n_dispatched = 0;

    while (!event_queue.empty()) {
        auto front_ev = std::move(event_queue.front());
        event_queue.pop_front();
        dispatch_event(*front_ev);
        n_dispatched++;
    }

    return n_dispatched;
}

std::size_t EventDispatcher::wrap_prio(std::size_t prefix, std::size_t prio) {
    constexpr auto sizet_bits = std::numeric_limits<std::size_t>::digits;
    constexpr auto shift_bits = sizet_bits / 2;
    constexpr auto low_mask = (std::size_t(1) << shift_bits) - 1;
    auto clamped_prefix = std::min(prefix, low_mask);
    auto clamped_prio = std::min(prio, low_mask);

    return ((clamped_prefix & low_mask) << shift_bits) |
           (clamped_prio & low_mask);
}

std::pair<std::size_t, std::size_t>
EventDispatcher::unwrap_prio(std::size_t wprio) {
    constexpr auto sizet_bits = std::numeric_limits<std::size_t>::digits;
    constexpr auto shift_bits = sizet_bits / 2;
    constexpr auto low_mask = (std::size_t(1) << shift_bits) - 1;

    return {wprio >> shift_bits, wprio & low_mask};
}

bool EventDispatcher::has_priority_set(const Event::KeyView &event_name) {
    return event_observer_map.contains(std::string(event_name));
}

EventDispatcher::ObserverPrioSet &
EventDispatcher::get_priority_set(const Event::KeyView &event_name) {
    return event_observer_map[std::string(event_name)];
}

std::optional<EventDispatcher::ObserverQuery>
EventDispatcher::find_observer(const std::string_view &observer_name,
                               const Event::KeyView &event_name) {
    const auto event_str = std::string(event_name);

    if (!event_observer_map.contains(event_str))
        return std::nullopt;

    auto &obs_set = event_observer_map.at(event_str);
    auto observer =
        std::find_if(obs_set.cbegin(), obs_set.cend(),
                     [&observer_name](const PriorityObserverKey &key) {
                         return key.name == observer_name;
                     });

    if (observer != obs_set.cend())
        return std::make_optional<EventDispatcher::ObserverQuery>(obs_set,
                                                                  observer);
    else
        return std::nullopt;
}

EventDispatcherStatusPair EventDispatcher::dispatch_event_to_observer(
    ObserverPrioSet &obs_set, ObserverPrioSet::const_iterator observer_iter,
    const Event &ev) {
    auto shared_p = observer_iter->observer.lock();
    if (shared_p != nullptr) {
        return {true, shared_p->on_event(ev)};
    } else {
        // weak pointer to an observer is expired so remove it
        // from the observer set.
        remove_observer_from_set(ev.name, observer_iter);
        return {false, ObserverReturnSignal::CONTINUE};
    }
}

bool EventDispatcher::dispatch_event(const Event &ev) {

    if (has_priority_set(ev.name))
        return false;

    bool any_notified = false;

    auto &prio_set = get_priority_set(ev.name);
    for (auto iter = prio_set.cbegin(); iter != prio_set.cend(); iter++) {
        auto result = dispatch_event_to_observer(prio_set, iter, ev);
        if (result.first) {
            any_notified = true;
            if (should_event_be_dropped(result.second))
                break;
        }
    }
    return any_notified;
}

void EventDispatcher::remove_observer_from_set(
    const Event::KeyView &obs_set_key, ObserverPrioSet::const_iterator iter) {
    auto key_str = std::string(obs_set_key);
    auto &obs_set = event_observer_map.at(key_str);
    obs_set.erase(iter);
    if (obs_set.empty())
        event_observer_map.erase(key_str);
}

bool EventDispatcher::should_event_be_dropped(ObserverReturnSignal signal) {
    return signal == ObserverReturnSignal::DROP_EVENT;
}

} // namespace plane_quest::engine