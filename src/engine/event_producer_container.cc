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

#include "event_producer_container.hh"
#include "event_dispatcher.hh"

namespace redseen::engine {

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

} // namespace redseen::engine