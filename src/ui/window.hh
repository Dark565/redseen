#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <chrono>

struct GLFWwindow;

namespace plane_quest::render {
class Drawer;
}

namespace plane_quest::ui {

class WindowEventLoop;

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

    bool
    pullEventToEventLoop(const std::shared_ptr<WindowEventLoop> &el,
                         const std::chrono::duration<std::size_t> &timeout);

  private:
    std::unique_ptr<WindowImpl> impl; // Use unique_ptr for PIMPL
};

} // namespace plane_quest::ui
