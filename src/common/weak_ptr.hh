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