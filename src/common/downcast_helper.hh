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

#include <concepts>

#include <type_traits>
#include <utility>
namespace redseen {

/** This class is used as an efficient alternative for RTTI dynamic_cast.
Subclasses identify themselves by key which is hashed */
template <class Base, class Key = std::string_view> class DowncastHelper {
    std::size_t type_hash;

  protected:
    DowncastHelper(const Key &type_key) {
        type_hash = std::hash<Key>{}(type_key);
    }

  public:
    template <std::derived_from<Base> T> T &&try_downcast() {
        std::size_t downcast_type_hash = std::hash<Key>{}(T::type_key());
        if (this->type_hash != downcast_type_hash)
            throw std::bad_cast();

        return static_cast<T &&>(*this);
    }
};

} // namespace redseen