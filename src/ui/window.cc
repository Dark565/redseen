#include "window.hh"
#include "render/drawer.hh"
#include "window_impl.hh"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace plane_quest::ui {

WindowImpl::WindowImpl(const WindowConfig &conf)
    : window(nullptr) { // Initialize TextureDrawer with required arguments
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

render::Drawer &Window::getDrawer() {
    return impl->getDrawer();
} // Corrected return type

} // namespace plane_quest::ui
