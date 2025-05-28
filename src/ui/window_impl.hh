#pragma once

#include <optional>
#include <GLFW/glfw3.h>

#include "render/opengl_drawer.hh"
#include "window.hh"

namespace plane_quest::ui {

class WindowImpl {
  public:
    WindowImpl(const WindowConfig &conf);
    ~WindowImpl();

    void show();
    void hide();
    render::Drawer &getDrawer();
    void *getNativeHandle() const;

  private:
    GLFWwindow *window = nullptr;
    std::optional<render::OpenGLDrawer> drawer;
    bool visible;
};

} // namespace plane_quest::ui
