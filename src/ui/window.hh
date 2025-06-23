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
