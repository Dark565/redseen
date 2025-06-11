#pragma once

namespace plane_quest {

/** A mixin that makes a derived class non-copyable (but movable). */
struct NonCopyable {
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;

    NonCopyable(NonCopyable &&) = default;
    NonCopyable &operator=(NonCopyable &&) = default;

  protected:
    NonCopyable() = default;
};

} // namespace plane_quest