#pragma once

#include <GLFW/glfw3.h>
#include "window_config.hh"
#include "window.hh"

namespace plane_quest {
namespace ui {

class WindowImpl {
public:
    render::Drawer drawer; // Updated type
    bool visible = false;

    WindowImpl(const WindowConfig& conf);
    ~WindowImpl();
    void show();
    void hide();
    render::Drawer& getDrawer();

private:
    GLFWwindow* window = nullptr; // Add window member
};

} // namespace ui
} // namespace plane_quest
