#pragma once

#include <memory>
#include <optional>
#include <chrono>
#include <GLFW/glfw3.h>

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
    render::Drawer &getDrawer();
    void *getNativeHandle() const;

    bool
    pullEventToEventLoop(const std::shared_ptr<WindowEventLoop> &el,
                         const std::chrono::duration<std::size_t> &timeout);

  private:
    GLFWwindow *window = nullptr;
    std::optional<render::OpenGLDrawer> drawer;
    bool visible;

    std::shared_ptr<WindowEventLoop> waiting_event_loop;
    bool any_event_pulled = false;

    static void glfw_ev_key_callback(GLFWwindow *window, int key, int scancode,
                                     int action, int mods);

    static void glfw_ev_mouse_button_callback(GLFWwindow *window, int button,
                                              int action, int mods);

    static void glfw_ev_cursor_position_callback(GLFWwindow *window,
                                                 double xpos, double ypos);
};

} // namespace plane_quest::ui
