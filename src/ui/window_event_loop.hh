#pragma once

#include <memory>

#include "engine/event_loop.hh"
#include <memory>

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
    WindowEventLoop(const std::shared_ptr<plane_quest::ui::Window> &window);

    friend class WindowImpl;
};

} // namespace plane_quest::ui