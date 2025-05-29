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
            glm::radians(45.0f),
            static_cast<float>(config.width) / config.height, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0),
                                     glm::vec3(0, 1, 0));
        glm::mat4 vp = projection * view;

        // Main loop
        while (!glfwWindowShouldClose(
            static_cast<GLFWwindow *>(window.getNativeHandle()))) {
            textureDrawer->clear(0.2f, 0.2f, 0.2f, 1.0f);

            // Instead of drawing the whole mesh at once, draw each glyph quad
            // separately with its own texture
            std::string text = "Deep Text!";
            float pen_x = 0.0f;
            for (size_t i = 0; i < text.size(); ++i) {
                char c = text[i];
                const plane_quest::render::Character &ch =
                    font->getCharacter(c);
                // Use the glyph's OpenGL texture ID directly
                // plane_quest::render::Texture glyphTexture(
                //     ch.textureID, ch.size.x, ch.size.y, false);

                // Build a single-quad mesh for this glyph (remove pen_x from
                // vertex positions)
                plane_quest::render::TextMesh glyphMesh;
                float xpos = ch.bearing.x * 1.0f;
                float ypos = ch.bearing.y * 1.0f;
                float w = ch.size.x * 1.0f;
                float h = ch.size.y * 1.0f;
                glyphMesh.vertices.push_back(
                    {glm::vec3(xpos, ypos - h, 0.0f), glm::vec2(0.0f, 1.0f)});
                glyphMesh.vertices.push_back(
                    {glm::vec3(xpos + w, ypos - h, 0.0f),
                     glm::vec2(1.0f, 1.0f)});
                glyphMesh.vertices.push_back(
                    {glm::vec3(xpos + w, ypos, 0.0f), glm::vec2(1.0f, 0.0f)});
                glyphMesh.vertices.push_back(
                    {glm::vec3(xpos, ypos, 0.0f), glm::vec2(0.0f, 0.0f)});
                glyphMesh.indices = {0, 1, 2, 2, 3, 0};

                // Animate rotation and scaling as before
                float time = static_cast<float>(glfwGetTime());
                glm::mat4 model = glm::mat4(1.0f);
                // 1. Translate by pen_x (in pixel units)
                model = glm::translate(model, glm::vec3(pen_x, 0.0f, 0.0f));
                // 2. Center the text (global offset, in pixel units)
                model = glm::translate(model, glm::vec3(-100.0f, -20.0f, 0.0f));
                // 3. Scale to world units
                model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
                // 4. Rotate
                model = glm::rotate(model, glm::radians(-20.0f),
                                    glm::vec3(1, 0, 0));
                model = glm::rotate(model, time, glm::vec3(0, 0, 1));
                // Offset each glyph in X by pen_x * scale
                model = glm::translate(
                    model, glm::vec3(-100.0f + pen_x * 0.02f, -20.0f, 0.0f));

                // Draw front face for this glyph
                meshRenderer.render(glyphMesh, projection, view * model,
                                    glm::vec3(0.0f, 1.0f, 1.0f), ch.textureID);

                // Draw back face (mirrored in z, slightly offset)
                glm::mat4 backModel =
                    model *
                    glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2.0f));
                backModel = backModel *
                            glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));
                meshRenderer.render(glyphMesh, projection, view * backModel,
                                    glm::vec3(0.0f, 0.5f, 1.0f), ch.textureID);

                pen_x += (ch.advance >> 6) * 1.0f;
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