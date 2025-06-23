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

#include <cstddef>
#include <string_view>

#include <GLFW/glfw3.h>

#include "engine/event.hh"

namespace plane_quest::ui {

namespace window_event {

constexpr std::string_view MOUSE_MOVE = "window.mouse.move";
constexpr std::string_view MOUSE_BUTTON_CLICK = "window.button.click";
constexpr std::string_view FOCUS = "window.focus";
constexpr std::string_view KEY = "window.key";

enum class Action { PRESS, RELEASE };

struct WindowEvent : engine::Event {
    using engine::Event::Event;
};

struct MouseMoveEvent : WindowEvent {
    size_t x, y;

    MouseMoveEvent(size_t x, size_t y) : WindowEvent(MOUSE_MOVE), x(x), y(y) {}
};

struct MouseButtonClick : WindowEvent {
    enum class Button { LEFT = 0, RIGHT = 1, MIDDLE = 2 } button;
    Action action;
    MouseButtonClick(Button button, Action action)
        : WindowEvent(MOUSE_BUTTON_CLICK), button(button), action(action) {}
};

struct Focus : WindowEvent {
    Focus() : WindowEvent(FOCUS) {}
};

struct Key : WindowEvent {
    int key;
    Action action;

    Key(int key, Action action) : WindowEvent(KEY), key(key), action(action) {}
};

} // namespace window_event

using window_event::WindowEvent;

} // namespace plane_quest::ui