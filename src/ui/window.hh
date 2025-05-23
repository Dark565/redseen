#pragma once

#include <memory>

#include "render/drawer.hh" // Use the real render module
#include "window_config.hh"

namespace plane_quest::ui {

class Window {
    std::unique_ptr<class WindowImpl> impl;

  public:
    Window(const WindowConfig &conf);

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&) noexcept = default;
    Window &operator=(Window &&) noexcept = default;

    void show();
    void hide();
    render::Drawer &getDrawer(); // Updated to use render::Drawer
};

} // namespace plane_quest::ui
