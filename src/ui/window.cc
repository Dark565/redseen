#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "render/drawer.hh"
#include "window_impl.hh"

namespace plane_quest::ui {

Window::Window(const WindowConfig &conf)
    : impl(std::make_unique<WindowImpl>(conf)) {}

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
}

WindowImpl::~WindowImpl() {
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
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

void Window::show() { impl->show(); }

void Window::hide() { impl->hide(); }

render::Drawer &Window::getDrawer() {
    return impl->getDrawer();
} // Corrected return type

} // namespace plane_quest::ui
