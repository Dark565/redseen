#pragma once

#include <memory>

#include "ui/drawer_stub.hh" // Ensure DrawerStub is included
#include "window_config.hh"

namespace plane_quest::ui {

class DrawerStub {
public:
    virtual ~DrawerStub() = default;
    virtual void clear(float r, float g, float b, float a = 1.0f) {}
    virtual void present() {}
};

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
