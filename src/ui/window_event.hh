#pragma once

#include <cstddef>

#include <GLFW/glfw3.h>

#include "engine/event.hh"

namespace plane_quest::ui {

namespace window_event {
enum class Type {
    MOUSE_MOVE = 0x1,
    MOUSE_BUTTON_CLICK = 0x2,
    FOCUS = 0x4,
    KEY = 0x8
};

enum class Action { PRESS, RELEASE };

struct WindowEvent : engine::Event {
    WindowEvent() = default;
    Type window_event_type;
};

struct MouseMoveEvent : WindowEvent {
    size_t x, y;
};

struct MouseButtonClick : WindowEvent {
    enum class Button { LEFT = 0, RIGHT = 1, MIDDLE = 2 } button;
    Action action;
};

struct Focus : WindowEvent {};

struct Key : WindowEvent {
    int key;
    Action action;
};

} // namespace window_event

using window_event::WindowEvent;

} // namespace plane_quest::ui