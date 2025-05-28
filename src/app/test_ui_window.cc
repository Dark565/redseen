#include "ui/window.hh"
#include "render/text.hh" // Add this include for text rendering
#include "render/texture_drawer.hh"
#include <GLFW/glfw3.h> // Include GLFW for event handling
#include <iostream>
#include <cmath>       // For trigonometric functions
#include <glad/glad.h> // Include OpenGL types like GLuint
#include <unistd.h>    // For getcwd
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono> // For high_resolution_clock

int main() {
    std::cout << "Program execution started." << std::endl;

    try {
        plane_quest::ui::WindowConfig config;
        config.width = 800;
        config.height = 600;
        config.name = "Rotating Text Example";

        std::cout << "Creating window..." << std::endl;
        plane_quest::ui::Window window(config);
        std::cout << "Window created. Showing window..." << std::endl;
        window.show();
        std::cout << "Window shown. Getting drawer..." << std::endl;
        auto &drawer = window.getDrawer();
        auto *textureDrawer =
            dynamic_cast<plane_quest::render::TextureDrawer *>(&drawer);
        if (!textureDrawer) {
            std::cerr << "Error: Drawer is not a TextureDrawer!" << std::endl;
            return 1;
        }

        // Enable blending for text rendering
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        std::cout << "Blending enabled." << std::endl;

        // Set up text rendering (use a system font path)
        std::string fontPath =
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
        unsigned int fontSize = 48;
        std::cout << "Loading font: " << fontPath << std::endl;
        plane_quest::render::Text textRenderer(fontPath, fontSize);
        std::cout << "Text renderer created." << std::endl;

        // Check if font loaded successfully (Text class should expose a method or status)
        // If not, print a warning (pseudo-code, adjust if Text exposes such a method)
        // if (!textRenderer.isLoaded()) {
        //     std::cerr << "Font failed to load!" << std::endl;
        // }

        // Set up orthographic projection to match window size for text
        // rendering (TextImpl will use this projection) Provide a public method
        // in Text to set the projection matrix
        textRenderer.setProjection(
            glm::ortho(0.0f, float(config.width), float(config.height), 0.0f));

        // Animation state
        static auto lastTime = std::chrono::high_resolution_clock::now();
        static float angle = 0.0f;
        const float fixedTimeStep = 1.0f / 60.0f; // 60Hz
        float accumulator = 0.0f;

        while (!glfwWindowShouldClose(
            static_cast<GLFWwindow *>(window.getNativeHandle()))) {
            textureDrawer->clear(0.1f, 0.1f, 0.1f, 1.0f);
            glBindFramebuffer(
                GL_FRAMEBUFFER,
                0); // Ensure default framebuffer is bound after clear

            // Calculate delta time
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime =
                std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            accumulator += deltaTime;

            // Update rotation
            while (accumulator >= fixedTimeStep) {
                angle += 720.0f * fixedTimeStep; // 720 degrees per second (2Hz)
                if (angle > 360.0f)
                    angle -= 360.0f;
                accumulator -= fixedTimeStep;
            }

            // Compute text position (centered)
            float x = config.width / 2.0f;
            float y = config.height / 2.0f;
            float scale = 1.0f;
            glm::vec3 color(1.0f, 1.0f, 0.0f); // Yellow

            // Center of rotation (text center)
            float textWidth = 400.0f; // Approximate width for centering
            float textHeight = 48.0f; // Font size
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x, y, 0.0f));
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(
                model, glm::vec3(-textWidth / 2.0f, -textHeight / 2.0f, 0.0f));

            // DEBUG: Print angle and model matrix
            std::cout << "angle: " << angle << std::endl;
            std::cout << "model matrix: [" << model[0][0] << ", " << model[0][1]
                      << ", " << model[0][2] << ", " << model[0][3] << "; "
                      << model[1][0] << ", " << model[1][1] << ", "
                      << model[1][2] << ", " << model[1][3] << "; "
                      << model[2][0] << ", " << model[2][1] << ", "
                      << model[2][2] << ", " << model[2][3] << "; "
                      << model[3][0] << ", " << model[3][1] << ", "
                      << model[3][2] << ", " << model[3][3] << "]" << std::endl;

            // DEBUG: Try rendering text at a fixed position with identity model matrix
            textRenderer.renderText("Rotating Example Text", 100.0f, 100.0f, scale,
                                    color, glm::mat4(1.0f));
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "OpenGL error after text rendering: " << err
                          << std::endl;
            } else {
                std::cout << "No OpenGL error after text rendering."
                          << std::endl;
            }

            textureDrawer->present();
            glfwPollEvents();
        }
        window.hide();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
