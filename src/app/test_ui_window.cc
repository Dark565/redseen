#include "ui/window.hh"
#include "render/text.hh"
#include "render/texture_drawer.hh"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unistd.h> // For access() and F_OK

void checkGLError(const char *location) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << location << ": 0x" << std::hex
                  << error << std::dec << std::endl;
    }
}

// Helper function to check if a file exists and is readable
bool checkFileExists(const std::string &path) {
    if (access(path.c_str(), F_OK | R_OK) == -1) {
        std::cerr << "File does not exist or is not readable: " << path
                  << std::endl;
        return false;
    }
    std::cout << "File exists and is readable: " << path << std::endl;
    return true;
}

int main() {
    try {
        // Create window
        plane_quest::ui::WindowConfig config;
        config.width = 800;
        config.height = 600;
        config.name = "Simple Text Test";

        std::cout << "Creating window..." << std::endl;
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

        // Basic OpenGL setup
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        checkGLError("After blend setup");

        // Create text renderer
        std::cout << "Creating text renderer..." << std::endl;
        std::string fontPath =
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
        if (!checkFileExists(fontPath)) {
            std::cerr << "Font file not found!" << std::endl;
            return 1;
        }

        plane_quest::render::Text textRenderer(fontPath, 32);
        std::cout << "Text renderer created" << std::endl;

        // Setup simple orthographic projection (Y from bottom to top)
        glm::mat4 projection =
            glm::ortho(0.0f,                              // left
                       static_cast<float>(config.width),  // right
                       0.0f,                              // bottom
                       static_cast<float>(config.height), // top
                       -1.0f,                             // near
                       1.0f                               // far
            );
        textRenderer.setProjection(projection);
        std::cout << "Projection matrix set" << std::endl;

        // Main loop
        while (!glfwWindowShouldClose(
            static_cast<GLFWwindow *>(window.getNativeHandle()))) {
            // Clear with a visible color to confirm rendering is happening
            textureDrawer->clear(0.3f, 0.3f, 0.3f, 1.0f);
            checkGLError("After clear");

            // Simple identity model matrix
            glm::mat4 model(1.0f);

            // Position text near the center of the screen
            const std::string text = "Test Text";
            float textX = config.width / 4.0f;  // Quarter from the left
            float textY = config.height / 2.0f; // Middle of the screen
            float scale = 1.0f;
            glm::vec3 color(1.0f, 1.0f, 1.0f); // White

            textRenderer.renderText(text, textX, textY, scale, color, model);
            checkGLError("After renderText");

            textureDrawer->present();
            checkGLError("After present");

            glfwPollEvents();
        }

        window.hide();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
