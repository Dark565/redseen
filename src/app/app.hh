#pragma once

#include <memory>
#include <string_view>

namespace plane_quest {

class App {
    std::unique_ptr<class AppImpl> impl;

  public:
    App(const std::string_view &name);
    void run();
};

} // namespace plane_quest
