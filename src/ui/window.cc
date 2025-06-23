/*
 *  Copyright (C) 2025 Grzegorz Kociołek (grzegorz.kclk@gmail.com)
 *
 *  This file is a part of RedSeen; a 3D game engine.
 *
 *  RedSeen is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RedSeen is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "engine/event_dispatcher.hh"
#include "window_impl.hh"

#include <endian.h>
#include <stdexcept>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "render/drawer.hh"

#include "ui/window_event.hh"

namespace plane_quest::ui {

Window::Window(const WindowConfig &conf)
    : impl(std::make_unique<WindowImpl>(conf)) {}

Window::~Window() = default;

void Window::show() { impl->show(); }
void Window::hide() { impl->hide(); }
const std::shared_ptr<render::OpenGLDrawer> &Window::getDrawer() const {
    return impl->getDrawer();
}
void *Window::getNativeHandle() const { return impl->getNativeHandle(); }

std::size_t Window::feed_dispatcher(engine::EventDispatcher &disp,
                                    bool can_block) {
    return impl->feed_dispatcher(disp, can_block);
}

void WindowImpl::init_callbacks() {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, glfw_ev_key_callback);
    glfwSetMouseButtonCallback(window, glfw_ev_mouse_button_callback);
    glfwSetCursorPosCallback(window, glfw_ev_cursor_position_callback);
}

void WindowImpl::glfw_ev_key_callback(GLFWwindow *window, int key, int scancode,
                                      int action, int mods) {

    auto impl = static_cast<WindowImpl *>(glfwGetWindowUserPointer(window));

    auto disp = impl->pending_dispatcher;
    if (disp == nullptr)
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

    auto event = std::make_shared<window_event::Key>(key, ev_action);
    disp->queue_last(std::move(event));

    impl->n_queued++;
}

void WindowImpl::glfw_ev_mouse_button_callback(GLFWwindow *window, int button,
                                               int action, int mods) {
    auto impl = static_cast<WindowImpl *>(glfwGetWindowUserPointer(window));

    auto disp = impl->pending_dispatcher;
    if (disp == nullptr)
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

    auto event =
        std::make_shared<window_event::MouseButtonClick>(ev_button, ev_action);
    disp->queue_last(std::move(event));
    impl->n_queued++;
}

void WindowImpl::glfw_ev_cursor_position_callback(GLFWwindow *window,
                                                  double xpos, double ypos) {
    auto impl = static_cast<WindowImpl *>(glfwGetWindowUserPointer(window));

    auto disp = impl->pending_dispatcher;
    if (disp == nullptr)
        return;

    auto event = std::make_shared<window_event::MouseMoveEvent>(
        static_cast<std::size_t>(xpos), static_cast<std::size_t>(ypos));
    disp->queue_last(std::move(event));
    impl->n_queued++;
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
    drawer =
        std::make_shared<render::OpenGLDrawer>(conf.width, conf.height, window);

    init_callbacks();
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

const std::shared_ptr<render::OpenGLDrawer> &WindowImpl::getDrawer() const {
    return drawer;
}

void *WindowImpl::getNativeHandle() const { return window; }

std::size_t WindowImpl::feed_dispatcher(engine::EventDispatcher &disp,
                                        bool can_block) {
    pending_dispatcher = &disp;
    n_queued = 0;
    if (can_block)
        glfwWaitEvents();
    else
        glfwPollEvents();

    this->pending_dispatcher = nullptr;
    return n_queued;
}

} // namespace plane_quest::ui
