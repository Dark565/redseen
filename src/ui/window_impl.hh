#pragma once

#include <GLFW/glfw3.h>

#include "render/drawer.hh"
#include "window.hh"
#include "window_config.hh"

namespace plane_quest {
namespace ui {

class WindowImpl {
  public:
    std::unique_ptr<render::Drawer> drawer;
    bool visible = false;

    WindowImpl(const WindowConfig &conf);
    ~WindowImpl();
    void show();
    void hide();
    render::Drawer &getDrawer();
    GLFWwindow *getNativeHandle() const;

  private:
    GLFWwindow *window = nullptr;
};

} // namespace ui
} // namespace plane_quest
