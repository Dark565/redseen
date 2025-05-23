#include "ui/window.hh"
#include <iostream>

int main() {
    try {
        WindowConfig config;
        config.width = 800;
        config.height = 600;
        config.name = "Test Window";

        plane_quest::ui::Window window(config);
        window.show();

        std::cout << "Press Enter to close the window..." << std::endl;
        std::cin.get();

        window.hide();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
