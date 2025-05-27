#pragma once

#include <string>
#include <cstdint>
#include <memory>

struct GLFWwindow;

namespace plane_quest::render {
class Drawer;
}

namespace plane_quest::ui {

struct WindowConfig {
    uint32_t width;
    uint32_t height;
    std::string name;
};

class WindowImpl; // Forward declaration

class Window {
  public:
    Window(const WindowConfig &conf);
    ~Window();

    Window(const Window &) = delete;            // Disable copy constructor
    Window &operator=(const Window &) = delete; // Disable copy assignment

    Window(Window &&) = default;            // Enable move constructor
    Window &operator=(Window &&) = default; // Enable move assignment
    void show();
    void hide();
    render::Drawer &getDrawer();
    void *getNativeHandle() const;

  private:
    WindowImpl *impl; // Raw pointer for PIMPL
};

} // namespace plane_quest::ui
