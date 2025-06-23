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

#include <unordered_map>
#include <string>
#include <string_view>
#include "engine/event_dispatcher.hh"
#include "event_producer_container.hh"
#include "event_producer.hh"

namespace redseen::engine {
/** A class for event producers */
class EventProducerContainer {
    std::unordered_map<std::string, std::shared_ptr<EventProducer>> producers;

  public:
    std::size_t feed_dispatcher(EventDispatcher &, bool can_block = false);

    bool add_producer(const std::string_view &name,
                      std::shared_ptr<EventProducer> producer);
    bool remove_producer(const std::string_view &name);
};

} // namespace redseen::engine