#pragma once

#include <chrono>

namespace plane_quest::demos::particles {
constexpr std::size_t PRIORITY_CLASS = 1;
constexpr auto TICK_DELAY = std::chrono::milliseconds(16);
} // namespace plane_quest::demos::particles