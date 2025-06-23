/*
 *  Copyright (C) 2025 Grzegorz Kociołek (grzegorz.kclk@gmail.com)
 *
 *  This file is a part of RedSeen; a 3D game engine.
 *
 *  RedSeen is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RedSeen is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <functional>
#include <limits>
#include <optional>
#include <queue>
#include <unordered_map>
#include <set>
#include <string>
#include <string_view>
#include <memory>

#include "event.hh"
#include "event_observer.hh"

namespace redseen::engine {

class Event;
class Engine;

struct PriorityObserverKey {
    std::size_t prio;
    std::string name;
    std::weak_ptr<EventObserver> observer;
};

} // namespace redseen::engine

template <> struct std::less<redseen::engine::PriorityObserverKey> {
    bool operator()(const redseen::engine::PriorityObserverKey &a,
                    const redseen::engine::PriorityObserverKey &b) const {
        return std::less<decltype(a.prio)>{}(a.prio, b.prio);
    }
};

namespace redseen::engine {

using EventDispatcherStatusPair = std::pair<bool, ObserverReturnSignal>;

class EventDispatcher {
  public:
    using ObserverPrioSet = std::multiset<PriorityObserverKey>;
    using ObserverMap = std::unordered_map<Event::Key, ObserverPrioSet>;

    ObserverMap event_observer_map;
    std::deque<std::shared_ptr<Event>> event_queue;

  public:
    bool register_observer(const std::string_view &observer_name,
                           const Event::KeyView &event_name,
                           std::size_t priority_class, std::size_t priority,
                           const std::weak_ptr<EventObserver> &,
                           bool allow_duplicates = false);

    bool
    register_observer(const std::string_view &observer_name,
                      const std::initializer_list<Event::KeyView> &event_names,
                      std::size_t priority_class, std::size_t priority,
                      const std::weak_ptr<EventObserver> &observer,
                      bool allow_duplicates = false) {
        for (const auto &event_name : event_names) {
            register_observer(observer_name, event_name, priority_class,
                              priority, observer, allow_duplicates);
        }
        return true;
    }

    bool unregister_observer(const std::string_view &observer_name,
                             const Event::KeyView &event_name);

    bool set_observer_priority(const std::string_view &observer_name,
                               const Event::KeyView &event_name,
                               std::size_t prio,
                               bool include_duplicates = false);

    void queue_last(std::shared_ptr<Event>);
    void queue_next(std::shared_ptr<Event>);

    /** Drop all events in the queue */
    void drop_queue();

    std::size_t
    dispatch(std::size_t n = std::numeric_limits<std::size_t>::max());

    /** Wrap priority to be used as priority for registering observers.
    Engine's internal prefix is 0. */
    static std::size_t wrap_prio(size_t prefix, size_t prio);

    /** Unwrap priority used for registering. */
    static std::pair<std::size_t, std::size_t>
    unwrap_prio(std::size_t wrapped_prio);

  protected:
    using ObserverQuery =
        std::pair<ObserverPrioSet &, ObserverPrioSet::const_iterator>;

    bool has_priority_set(const Event::KeyView &event_name);

    /** Gets the priority set for a given event key. If it doesn't exist, a new
     * one is created. */
    ObserverPrioSet &get_priority_set(const Event::KeyView &event_name);

    std::optional<ObserverQuery>
    find_observer(const std::string_view &observer_name,
                  const Event::KeyView &event_name);

    EventDispatcherStatusPair
    dispatch_event_to_observer(ObserverPrioSet &obs_set,
                               ObserverPrioSet::const_iterator observer_iter,
                               const Event &ev);

    bool dispatch_event(const Event &ev);

    void remove_observer_from_set(const Event::KeyView &obs_set_key,
                                  ObserverPrioSet::const_iterator iter);

    /** Check if a given event should be dropped */
    static bool should_event_be_dropped(ObserverReturnSignal);

    /** Check if the event loop should be closed based on a status. */
    // bool should_be_closed(const EventDispatcherStatusPair &status);

    /** Get a status that an EventLoop should return on exit fron run(). */
    // bool return_status(const EventDispatcherStatusPair &status);
};

} // namespace redseen::engine