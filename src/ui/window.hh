#pragma once

#include <memory>

#include "window_config.hh"
#include "engine/render/drawer.hh"

namespace plane_quest::ui {

class Window {
    std::unique_ptr<class WindowImpl> impl;

  public:
    Window(const WindowConfig &conf);

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

		Window(Window&&) noexcept = default;
		Window& operator=(Window&&) noexcept = default;

    void show();
    void hide();
    render::Drawer &getDrawer();
};

} // namespace plane_quest::ui
