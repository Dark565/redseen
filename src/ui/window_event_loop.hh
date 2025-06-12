#pragma once

#include <memory>

#include "engine/event_loop.hh"
#include "window_event.hh"
#include <memory>

namespace plane_quest::ui {
class Window;
class WindowImpl;

class WindowEventLoop : public engine::EventLoop {
    std::shared_ptr<plane_quest::ui::Window> window;

  public:
    WindowEventLoop(const std::shared_ptr<plane_quest::ui::Window> &window);

    bool run() override;

    friend class WindowImpl;
};

} // namespace plane_quest::ui