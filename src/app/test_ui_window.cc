#include "ui/window.hh"
#include "render/texture_drawer.hh" // Include TextureDrawer for casting
#include "render/texture.hh" // Include Texture for loading the smiling face
#include "render/shader.hh"  // Include Shader for modern OpenGL
#include <GLFW/glfw3.h>      // Include GLFW for event handling
#include <iostream>
#include <cmath>       // For trigonometric functions
#include <glad/glad.h> // Include OpenGL types like GLuint
#include <unistd.h>    // For getcwd
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono> // For high_resolution_clock

GLuint createSmilingFaceTexture() {
    // Create a simple smiling face texture (corrected logic)
    const int width = 256;
    const int height = 256;
    unsigned char data[width * height * 4] = {};

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 4;
            float dx = x - width / 2.0f;
            float dy = y - height / 2.0f;
            float distance = sqrt(dx * dx + dy * dy);

            // Face (yellow circle)
            if (distance < width / 2.0f) {
                data[index] = 255;     // Red
                data[index + 1] = 255; // Green
                data[index + 2] = 0;   // Blue
                data[index + 3] = 255; // Alpha

                // Eyes (black circles)
                if ((dx > -50 && dx < -30 && dy > 30 && dy < 50) ||
                    (dx > 30 && dx < 50 && dy > 30 && dy < 50)) {
                    data[index] = 0;
                    data[index + 1] = 0;
                    data[index + 2] = 0;
                }

                // Smile (black arc)
                if (dy > -30 && dy < -20 &&
                    dx * dx + (dy + 40) * (dy + 40) < 1600) {
                    data[index] = 0;
                    data[index + 1] = 0;
                    data[index + 2] = 0;
                }
            } else {
                data[index] = 0;
                data[index + 1] = 0;
                data[index + 2] = 0;
                data[index + 3] = 0; // Transparent
            }
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

int main() {
    std::cout << "Program execution started." << std::endl;

    try {
        plane_quest::ui::WindowConfig config;
        config.width = 800;
        config.height = 600;
        config.name = "Smiling Face";

        std::cout << "Creating window..." << std::endl;
        plane_quest::ui::Window window(config);
        std::cout << "Window created. Showing window..." << std::endl;
        window.show();
        std::cout << "Window shown. Getting drawer..." << std::endl;
        auto &drawer = window.getDrawer();
        std::cout << "Drawer obtained. Creating smiling face texture..."
                  << std::endl;

        // Create the smiling face texture
        GLuint smilingFaceTexture = createSmilingFaceTexture();

        // Set up VAO and VBO for rendering a quad
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        float vertices[] = {// Positions   // Texture Coords
                            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
                            1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f};

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Load and compile shaders
        std::cout << "Program started. Initializing shaders..." << std::endl;
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << "Current working directory: " << cwd << std::endl;
        } else {
            std::cerr << "Error: Unable to get current working directory."
                      << std::endl;
        }
        Shader shader(
            "/home/ubuntu/proj/planequest/src/render/vertex_shader.glsl",
            "/home/ubuntu/proj/planequest/src/render/fragment_shader.glsl");

        // Rendering loop
        static auto lastTime = std::chrono::high_resolution_clock::now();
        static float angle = 0.0f;
        const float fixedTimeStep = 1.0f / 30.0f; // Fixed time step for 30Hz
        float accumulator = 0.0f;

        while (!glfwWindowShouldClose(
            static_cast<GLFWwindow *>(window.getNativeHandle()))) {
            // Clear the screen
            drawer.clear(0.1f, 0.1f, 0.1f, 1.0f);

            // Calculate delta time
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime =
                std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            accumulator += deltaTime;

            // Update rotation at fixed intervals
            while (accumulator >= fixedTimeStep) {
                angle += 360.0f * fixedTimeStep; // Adjusted to 360 degrees per
                                                 // second for 1Hz rotation
                if (angle > 360.0f)
                    angle -= 360.0f;
                accumulator -= fixedTimeStep;
            }

            glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle),
                                          glm::vec3(0.0f, 0.0f, 1.0f));
            GLuint modelLoc = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            // Render the smiling face texture
            shader.use();
            glBindTexture(GL_TEXTURE_2D, smilingFaceTexture);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);

            // Present the frame
            drawer.present();

            // Poll for and process events
            glfwPollEvents();
        }

        glDeleteTextures(1, &smilingFaceTexture);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        window.hide();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
