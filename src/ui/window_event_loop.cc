#include "window_event_loop.hh"

#include "engine/event_loop.hh"
#include "ui/window.hh"

namespace plane_quest::app {

WindowEventLoop::WindowEventLoop(const std::shared_ptr<ui::Window> &window)
    : window(window) {}

bool WindowEventLoop::run() {}

} // namespace plane_quest::app