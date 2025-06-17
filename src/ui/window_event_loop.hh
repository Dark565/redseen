#pragma once

#include <chrono>
#include <memory>

#include "engine/event_loop.hh"
#include <memory>
#include <ratio>

namespace plane_quest::ui {
class Window;
class WindowImpl;

class WindowEventLoop : public engine::EventLoop,
                        std::enable_shared_from_this<WindowEventLoop> {
    std::shared_ptr<plane_quest::ui::Window> window;

  public:
    bool run() override;

    static std::shared_ptr<WindowEventLoop>
    create(const std::shared_ptr<plane_quest::ui::Window> &window);

  protected:
    WindowEventLoop(const std::shared_ptr<plane_quest::ui::Window> &window,
                    const std::chrono::microseconds &tickDelay);

    friend class WindowImpl;

  private:
    std::chrono::microseconds tickDelay;
};

} // namespace plane_quest::ui