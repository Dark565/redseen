#pragma once

#include <memory>
#include <optional>
#include <chrono>
#include <GLFW/glfw3.h>

#include "engine/event_loop.hh"
#include "render/opengl_drawer.hh"
#include "window.hh"

namespace plane_quest::ui {

class WindowEventLoop;
class WindowImpl {
  public:
    WindowImpl(const WindowConfig &conf);
    ~WindowImpl();

    void show();
    void hide();
    const std::shared_ptr<render::OpenGLDrawer> &getDrawer() const;
    void *getNativeHandle() const;

    std::optional<engine::EventLoopStatusPair>
    pullEvent(const std::shared_ptr<WindowEventLoop> &el,
              const std::chrono::microseconds &timeout);

    std::optional<engine::EventLoopStatusPair>
    pullEvent(const std::shared_ptr<WindowEventLoop> &el);

  private:
    GLFWwindow *window = nullptr;
    std::shared_ptr<render::OpenGLDrawer> drawer;
    bool visible;

    std::shared_ptr<WindowEventLoop> pending_event_loop;
    std::optional<engine::EventLoopStatusPair> event_loop_status;

    void init_callbacks();

    static void glfw_ev_key_callback(GLFWwindow *window, int key, int scancode,
                                     int action, int mods);

    static void glfw_ev_mouse_button_callback(GLFWwindow *window, int button,
                                              int action, int mods);

    static void glfw_ev_cursor_position_callback(GLFWwindow *window,
                                                 double xpos, double ypos);
};

} // namespace plane_quest::ui
