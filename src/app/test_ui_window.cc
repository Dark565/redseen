#include "ui/window.hh"
#include "render/text_renderer.hh"
#include "render/text.hh"
#include "render/texture_drawer.hh"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

void checkGLError(const char *location) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << location << ": 0x" << std::hex
                  << error << std::dec << std::endl;
    }
}

int main() {
    try {
        // Create window
        plane_quest::ui::WindowConfig config;
        config.width = 800;
        config.height = 600;
        config.name = "Text Rendering Test";

        plane_quest::ui::Window window(config);
        window.show();

        // Get drawer
        auto &drawer = window.getDrawer();
        auto *textureDrawer =
            dynamic_cast<plane_quest::render::TextureDrawer *>(&drawer);
        if (!textureDrawer) {
            std::cerr << "Failed to get TextureDrawer" << std::endl;
            return 1;
        }

        // Enable blending for text
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Create text renderer
        plane_quest::render::TextRenderer textRenderer;

        // Load font
        auto font = std::make_shared<plane_quest::render::Font>(
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 32);

        // Create text objects with different fonts/sizes
        auto text1 = textRenderer.createText(font);

        // Setup projection matrix
        glm::mat4 projection =
            glm::ortho(0.0f,                              // left
                       static_cast<float>(config.width),  // right
                       0.0f,                              // bottom
                       static_cast<float>(config.height), // top
                       -1.0f,                             // near
                       1.0f                               // far
            );
        textRenderer.setProjection(projection);

        // Main loop
        while (!glfwWindowShouldClose(
            static_cast<GLFWwindow *>(window.getNativeHandle()))) {
            textureDrawer->clear(0.2f, 0.2f, 0.2f, 1.0f);

            // Render different text examples
            text1->render("Hello, World!", 50.0f,
                          config.height - 50.0f, // Near top of screen
                          1.0f, glm::vec3(1.0f, 1.0f, 1.0f)); // White

            text1->render("Colored Text", 50.0f,
                          config.height - 100.0f,       // Below first text
                          1.2f,                         // Slightly larger
                          glm::vec3(1.0f, 1.0f, 0.0f)); // Yellow

            // Create a rotating model matrix for the third text
            float time = static_cast<float>(glfwGetTime());
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(
                model, glm::vec3(400.0f, 300.0f, 0.0f)); // Move to center
            model =
                glm::rotate(model, time, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate
            model = glm::translate(
                model, glm::vec3(-100.0f, 0.0f, 0.0f)); // Center the text

            text1->render("Rotating Text!", 0.0f,
                          0.0f, // Position will be transformed by model matrix
                          1.0f, glm::vec3(0.0f, 1.0f, 1.0f), // Cyan
                          model);

            textureDrawer->present();
            glfwPollEvents();
        }

        window.hide();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}