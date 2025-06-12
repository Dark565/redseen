#include "window_impl.hh"

#include <endian.h>
#include <stdexcept>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "render/drawer.hh"

#include "ui/window_event.hh"
#include "ui/window_event_loop.hh"

namespace plane_quest::ui {

Window::Window(const WindowConfig &conf)
    : impl(std::make_unique<WindowImpl>(conf)) {}

Window::~Window() = default;

void Window::show() { impl->show(); }
void Window::hide() { impl->hide(); }
render::Drawer &Window::getDrawer() { return impl->getDrawer(); }
void *Window::getNativeHandle() const { return impl->getNativeHandle(); }

void WindowImpl::glfw_ev_key_callback(GLFWwindow *window, int key, int scancode,
                                      int action, int mods) {

    auto impl = static_cast<WindowImpl *>(glfwGetWindowUserPointer(window));
    impl->any_event_pulled = true;

    auto el = impl->waiting_event_loop.get();
    if (el == nullptr)
        return;

    window_event::Action ev_action;

    switch (action) {
    case GLFW_PRESS:
    case GLFW_REPEAT:
        ev_action = window_event::Action::PRESS;
        break;
    case GLFW_RELEASE:
        ev_action = window_event::Action::RELEASE;
        break;
    }

    window_event::Key event = {.key = key, .action = ev_action};
    el->pass_event(event);
}

void WindowImpl::glfw_ev_mouse_button_callback(GLFWwindow *window, int button,
                                               int action, int mods) {
    auto impl = static_cast<WindowImpl *>(glfwGetWindowUserPointer(window));
    impl->any_event_pulled = true;

    auto el = impl->waiting_event_loop.get();
    if (el == nullptr)
        return;

    window_event::Action ev_action;

    switch (action) {
    case GLFW_PRESS:
    case GLFW_REPEAT:
        ev_action = window_event::Action::PRESS;
        break;
    case GLFW_RELEASE:
        ev_action = window_event::Action::RELEASE;
        break;
    }

    window_event::MouseButtonClick::Button ev_button;

    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        ev_button = window_event::MouseButtonClick::Button::LEFT;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        ev_button = window_event::MouseButtonClick::Button::RIGHT;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        ev_button = window_event::MouseButtonClick::Button::MIDDLE;
        break;
    }

    window_event::MouseButtonClick event = {.button = ev_button,
                                            .action = ev_action};

    el->pass_event(event);
}

void WindowImpl::glfw_ev_cursor_position_callback(GLFWwindow *window,
                                                  double xpos, double ypos) {
    auto impl = static_cast<WindowImpl *>(glfwGetWindowUserPointer(window));
    impl->any_event_pulled = true;

    auto el = impl->waiting_event_loop.get();
    if (el == nullptr)
        return;

    window_event::MouseMoveEvent event = {.x = static_cast<std::size_t>(xpos),
                                          .y = static_cast<std::size_t>(ypos)};

    el->pass_event(event);
}

WindowImpl::WindowImpl(const WindowConfig &conf) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    window = glfwCreateWindow(static_cast<int>(conf.width),
                              static_cast<int>(conf.height), conf.name.data(),
                              nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window); // Set OpenGL context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    drawer = std::make_optional<render::OpenGLDrawer>(conf.width, conf.height,
                                                      window);
}

WindowImpl::~WindowImpl() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}

void WindowImpl::show() {
    visible = true;
    glfwShowWindow(window);
}

void WindowImpl::hide() {
    visible = false;
    glfwHideWindow(window);
}

render::Drawer &WindowImpl::getDrawer() {
    return *drawer;
} // Corrected return type

void *WindowImpl::getNativeHandle() const { return window; }

bool WindowImpl::pullEventToEventLoop(
    const std::shared_ptr<WindowEventLoop> &el,
    const std::chrono::duration<std::size_t> &timeout) {
    waiting_event_loop = el;
    double timeout_sec = std::chrono::duration<double>(timeout).count();
    this->any_event_pulled = false;
    glfwWaitEventsTimeout(timeout_sec);
    waiting_event_loop = nullptr;
    return this->any_event_pulled;
}

} // namespace plane_quest::ui
