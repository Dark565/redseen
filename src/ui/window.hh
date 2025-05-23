#pragma once

#include <memory>

#include "render/drawer.hh" // Use the real render module
#include "window_config.hh"
#include "window_impl.hh" // Include full definition of WindowImpl

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
    render::Drawer &getDrawer();         // Updated to use render::Drawer
    GLFWwindow *getNativeHandle() const; // Declaration only
};

} // namespace plane_quest::ui
