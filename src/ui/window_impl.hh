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

#pragma once

#include <memory>
#include <GLFW/glfw3.h>

#include "engine/event_dispatcher.hh"
#include "render/opengl_drawer.hh"
#include "window.hh"

namespace redseen::ui {

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

} // namespace redseen::ui
