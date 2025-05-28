#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "render/drawer.hh"
#include "render/opengl_drawer.hh"

#include "window_impl.hh" // Only in implementation

namespace plane_quest::ui {

Window::Window(const WindowConfig &conf)
    : impl(std::make_unique<WindowImpl>(conf)) {}

Window::~Window() = default;

void Window::show() { impl->show(); }
void Window::hide() { impl->hide(); }
render::Drawer &Window::getDrawer() { return impl->getDrawer(); }
void *Window::getNativeHandle() const { return impl->getNativeHandle(); }

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

} // namespace plane_quest::ui
