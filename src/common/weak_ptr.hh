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

#include <memory>

namespace plane_quest {
/** Augmented std::weak_ptr that supports hashing. */
template <class T> class WeakPtr : public std::weak_ptr<T> {
    WeakPtr(const WeakPtr &) = default;
    WeakPtr(WeakPtr &&) = default;
    WeakPtr &operator=(const WeakPtr &) = default;
    WeakPtr &operator=(WeakPtr &&) = default;

    WeakPtr(const std::weak_ptr<T> &oth) : std::weak_ptr<T>(oth) {}
    WeakPtr(std::weak_ptr<T> &&oth) : std::weak_ptr<T>(std::move(oth)) {}

    WeakPtr &operator=(const std::weak_ptr<T> &oth) {
        std::weak_ptr<T>::operator=(oth);
        return *this;
    }

    WeakPtr &operator=(std::weak_ptr<T> &&oth) {
        std::weak_ptr<T>::operator=(std::move(oth));
        return *this;
    }
};
} // namespace plane_quest

template <class T> class std::hash<plane_quest::WeakPtr<T>> {
  public:
    std::size_t operator()(const plane_quest::WeakPtr<T> &wp) {
        auto sp = wp.lock();
        return std::hash<T *>{}(sp.get());
    }
};