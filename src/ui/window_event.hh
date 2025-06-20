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