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
    Type window_event_type;

    WindowEvent(Type window_event_type)
        : engine::Event(engine::EventType::EXTERNAL),
          window_event_type(window_event_type) {}
};

struct MouseMoveEvent : WindowEvent {
    size_t x, y;

    MouseMoveEvent(size_t x, size_t y)
        : WindowEvent(Type::MOUSE_MOVE), x(x), y(y) {}
};

struct MouseButtonClick : WindowEvent {
    enum class Button { LEFT = 0, RIGHT = 1, MIDDLE = 2 } button;
    Action action;
    MouseButtonClick(Button button, Action action)
        : WindowEvent(Type::MOUSE_BUTTON_CLICK), button(button),
          action(action) {}
};

struct Focus : WindowEvent {
    Focus() : WindowEvent(Type::FOCUS) {}
};

struct Key : WindowEvent {
    int key;
    Action action;

    Key(int key, Action action)
        : WindowEvent(Type::KEY), key(key), action(action) {}
};

} // namespace window_event

using window_event::WindowEvent;

} // namespace plane_quest::ui