#include <memory>
#include <optional>
#include <string_view>

#include "app.hh"
#include "ui/window.hh"

namespace plane_quest {

class AppImpl {
    std::optional<ui::Window> window;

  public:
    AppImpl(const std::string_view &name) {}

    void run() {}
};

plane_quest::App::App(const std::string_view &name)
    : impl(std::make_unique<AppImpl>(name)) {}

void plane_quest::App::run() { impl->run(); }

}; // namespace plane_quest
