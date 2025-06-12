#include "window_event_loop.hh"
#include "engine/event_loop.hh"
#include "window.hh"

#include <memory>
namespace plane_quest::ui {

class Window;

WindowEventLoop::WindowEventLoop(const std::shared_ptr<ui::Window> &window)
    : window(window) {}

std::shared_ptr<WindowEventLoop>
WindowEventLoop::create(const std::shared_ptr<ui::Window> &window) {
    return std::make_shared<WindowEventLoop>(window);
}

bool WindowEventLoop::run() {
    std::optional<engine::EventLoopStatusPair> el_status;
    auto shared_this = shared_from_this();
    do {
        el_status = window->pullEvent(shared_this);
    } while (!(el_status.has_value() && should_be_closed(*el_status)));
    return true;
}

} // namespace plane_quest::ui