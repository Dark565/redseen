#include "ui/window.hh"
#include "render/texture_drawer.hh" // Include TextureDrawer for casting
#include <GLFW/glfw3.h>             // Include GLFW for event handling
#include <iostream>

int main() {
    try {
        WindowConfig config;
        config.width = 800;
        config.height = 600;
        config.name = "Test Window";

        plane_quest::ui::Window window(config);
        window.show();

        auto &drawer = window.getDrawer();

        // Rendering loop
        while (!glfwWindowShouldClose(
            static_cast<GLFWwindow *>(window.getNativeHandle()))) {
            // Clear the screen
            drawer.clear(0.1f, 0.1f, 0.1f, 1.0f);

            // Present the frame
            drawer.present();

            // Poll for and process events
            glfwPollEvents();
        }

        window.hide();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
