#pragma once

namespace plane_quest {
struct DefaultMove {
    DefaultMove(DefaultMove &&) = default;
    DefaultMove &operator=(DefaultMove &&) = default;

  protected:
    DefaultMove() = default;
};
} // namespace plane_quest