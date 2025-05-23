#pragma once

#include "render/drawer.hh"
#include "window.hh"
#include "window_config.hh"
#include <GLFW/glfw3.h>

namespace plane_quest {
namespace ui {

class WindowImpl {
  public:
    std::unique_ptr<render::Drawer>
        drawer; // Use TextureDrawer as the concrete implementatio
    bool visible = false;

    WindowImpl(const WindowConfig &conf);
    ~WindowImpl();
    void show();
    void hide();
    render::Drawer &getDrawer();

  private:
    GLFWwindow *window = nullptr; // Add window member
};

} // namespace ui
} // namespace plane_quest
