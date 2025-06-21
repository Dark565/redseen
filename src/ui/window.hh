#pragma once

#include <exception>
#include <string>
#include <cstdint>
#include <memory>
#include <chrono>

#include "engine/event_dispatcher.hh"
#include "render/opengl_drawer.hh"

#include "engine/event_producer.hh"

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

class WindowImpl;

class Window : public engine::EventProducer,
               public std::enable_shared_from_this<Window> {
  public:
    Window(const WindowConfig &conf);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&) = default;
    Window &operator=(Window &&) = default;
    void show();
    void hide();
    const std::shared_ptr<render::OpenGLDrawer> &getDrawer() const;
    void *getNativeHandle() const;

    std::size_t feed_dispatcher(engine::EventDispatcher &,
                                bool can_block) override;

  private:
    std::unique_ptr<WindowImpl> impl; // Use unique_ptr for PIMPL
};

} // namespace plane_quest::ui
