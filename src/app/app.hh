#pragma once

#include <memory>
#include <string_view>

namespace plane_quest {

class AppImpl; // Forward declaration

class App {
    std::unique_ptr<AppImpl> impl; // Use unique_ptr for PIMPL

  public:
    App(const std::string_view &name);
    ~App();

    void run();
};

} // namespace plane_quest
