#include <chrono>
#include "window_event_loop.hh"
#include "engine/event.hh"
#include "engine/event_loop.hh"
#include "engine/event_observer.hh"
#include "window.hh"

#include <memory>
namespace plane_quest::ui {

class Window;

WindowEventLoop::WindowEventLoop(const std::shared_ptr<ui::Window> &window,
                                 const std::chrono::microseconds &tickDelay)
    : window(window), tickDelay(tickDelay) {}

std::shared_ptr<WindowEventLoop>
WindowEventLoop::create(const std::shared_ptr<ui::Window> &window) {
    return std::make_shared<WindowEventLoop>(window);
}

bool WindowEventLoop::run() {
    std::optional<engine::EventLoopStatusPair> el_status;
    auto beg_time = std::chrono::steady_clock::now();
    auto shared_this = shared_from_this();
    do {
        auto time_now = std::chrono::steady_clock::now();
        auto time_delta = time_now - beg_time;
        for (auto time = time_delta - tickDelay;
             time >= std::chrono::microseconds(0); time -= tickDelay) {
            pass_event(engine::Event{engine::EventType::TICK});
        }
        beg_time = time_now;

        el_status = window->pullEvent(shared_this, tickDelay);
    } while (!(el_status.has_value() && should_be_closed(*el_status)));
    return return_status(*el_status);
}

} // namespace plane_quest::ui