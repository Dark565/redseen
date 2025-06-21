#pragma once

#include <memory>
#include <GLFW/glfw3.h>

#include "engine/event_dispatcher.hh"
#include "render/opengl_drawer.hh"
#include "window.hh"

namespace plane_quest::ui {

class WindowImpl {
  public:
    WindowImpl(const WindowConfig &conf);
    ~WindowImpl();

    void show();
    void hide();
    const std::shared_ptr<render::OpenGLDrawer> &getDrawer() const;
    void *getNativeHandle() const;

    std::size_t feed_dispatcher(engine::EventDispatcher &, bool can_block);

  private:
    GLFWwindow *window = nullptr;
    std::shared_ptr<render::OpenGLDrawer> drawer;
    bool visible;

    engine::EventDispatcher *pending_dispatcher = nullptr;
    std::size_t n_queued = 0;

    void init_callbacks();

    static void glfw_ev_key_callback(GLFWwindow *window, int key, int scancode,
                                     int action, int mods);

    static void glfw_ev_mouse_button_callback(GLFWwindow *window, int button,
                                              int action, int mods);

    static void glfw_ev_cursor_position_callback(GLFWwindow *window,
                                                 double xpos, double ypos);
};

} // namespace plane_quest::ui
