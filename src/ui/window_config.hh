#pragma once

#include <cstddef>
#include <string_view>

struct WindowConfig {
    std::string_view name;
    size_t width;
    size_t height;
};
