#include "ui/window.hh"
#include "render/text_renderer.hh"
#include "render/text.hh"
#include "render/opengl_drawer.hh"
#include "render/mesh_renderer.hh"
#include "render/text_mesh.hh"
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
            dynamic_cast<plane_quest::render::OpenGLDrawer *>(&drawer);
        if (!textureDrawer) {
            std::cerr << "Failed to get OpenGLDrawer" << std::endl;
            return 1;
        }

        // Enable blending for text
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Disable face culling - we want to see both sides
        glDisable(GL_CULL_FACE);

        // Create text renderer
        plane_quest::render::TextRenderer textRenderer;

        // Load font
        auto font = std::make_shared<plane_quest::render::Font>(
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 32);

        // Create text objects with different fonts/sizes
        auto text1 = textRenderer.createText(font);

        // Create a mesh for the text
        auto textMesh =
            plane_quest::render::TextMesh::fromString("Deep Text!", font, 1.0f);
        plane_quest::render::MeshRenderer meshRenderer;

        // Setup projection matrix for 3D
        glm::mat4 projection = glm::perspective(
            glm::radians(60.0f),
            static_cast<float>(config.width) / config.height, 0.1f, 100.0f);

        // Position camera at a more dramatic angle
        glm::mat4 view = glm::lookAt(glm::vec3(20.0f, 15.0f, 25.0f),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));

        // Main loop
        while (!glfwWindowShouldClose(
            static_cast<GLFWwindow *>(window.getNativeHandle()))) {
            textureDrawer->clear(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float time = static_cast<float>(glfwGetTime());
            float scale = 0.15f;
            float depth = 5.0f;

            // Create the shared rotation transformation
            glm::mat4 sharedTransform = glm::mat4(1.0f);
            sharedTransform = glm::rotate(sharedTransform, glm::radians(-30.0f),
                                          glm::vec3(1, 0, 0));
            sharedTransform = glm::rotate(sharedTransform, time * 0.8f,
                                          glm::vec3(0, 1, 0.3f));

            // Compute total width for centering
            float total_width = 0.0f;
            std::string text = "Deep Text!";
            for (size_t i = 0; i < text.size(); ++i) {
                const plane_quest::render::Character &ch =
                    font->getCharacter(text[i]);
                total_width += (ch.advance >> 6);
            }
            float pen_x = -(total_width * scale) / 2.0f;

            for (size_t i = 0; i < text.size(); ++i) {
                char c = text[i];
                const plane_quest::render::Character &ch =
                    font->getCharacter(c);
                float xpos = ch.bearing.x * 1.0f;
                float ypos = ch.bearing.y * 1.0f;
                float w = ch.size.x * 1.0f;
                float h = ch.size.y * 1.0f;

                // Create textured mesh for front/back faces
                plane_quest::render::TextMesh texturedMesh;
                texturedMesh.vertices = {
                    // Front face vertices (z = 0)
                    {glm::vec3(xpos, ypos - h, 0.0f),
                     glm::vec2(0.0f, 1.0f)}, // 0
                    {glm::vec3(xpos + w, ypos - h, 0.0f),
                     glm::vec2(1.0f, 1.0f)}, // 1
                    {glm::vec3(xpos + w, ypos, 0.0f),
                     glm::vec2(1.0f, 0.0f)},                              // 2
                    {glm::vec3(xpos, ypos, 0.0f), glm::vec2(0.0f, 0.0f)}, // 3

                    // Back face vertices (z = -depth)
                    {glm::vec3(xpos, ypos - h, -depth),
                     glm::vec2(0.0f, 1.0f)}, // 4
                    {glm::vec3(xpos + w, ypos - h, -depth),
                     glm::vec2(1.0f, 1.0f)}, // 5
                    {glm::vec3(xpos + w, ypos, -depth),
                     glm::vec2(1.0f, 0.0f)},                               // 6
                    {glm::vec3(xpos, ypos, -depth), glm::vec2(0.0f, 0.0f)} // 7
                };

                // Indices for front and back faces
                texturedMesh.indices = {// Front face
                                        0, 1, 2, 0, 2, 3,

                                        // Back face
                                        4, 5, 6, 4, 6, 7};

                // Create solid mesh for side faces
                plane_quest::render::TextMesh solidMesh;
                solidMesh.vertices = texturedMesh.vertices; // Reuse vertices

                // Indices for side faces
                solidMesh.indices = {// Left face
                                     0, 4, 7, 0, 7, 3,

                                     // Right face
                                     1, 2, 6, 1, 6, 5,

                                     // Top face
                                     3, 2, 6, 3, 6, 7,

                                     // Bottom face
                                     0, 1, 5, 0, 5, 4};

                // Individual glyph transformation
                glm::mat4 model = sharedTransform;
                model = glm::scale(model, glm::vec3(scale));
                model =
                    glm::translate(model, glm::vec3(pen_x / scale, 0.0f, 0.0f));

                // Draw textured faces (front and back)
                glm::vec3 frontColor(0.0f, 1.0f,
                                     1.0f); // Bright cyan for front/back
                meshRenderer.render(texturedMesh, projection, view * model,
                                    frontColor, ch.textureID);

                // Draw solid faces (sides)
                glm::vec3 sideColor(0.0f, 0.6f, 0.6f); // Darker cyan for sides
                meshRenderer.render(solidMesh, projection, view * model,
                                    sideColor,
                                    0); // Use texture ID 0 for solid color

                // Advance pen_x
                float advance = (ch.advance >> 6);
                if (advance == 0.0f) {
                    advance = ch.size.x > 0 ? ch.size.x : 16.0f;
                }
                pen_x += advance * scale;
            }

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