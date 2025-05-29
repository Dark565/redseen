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
#include <GL/glu.h>
#include <array>
#include <mapbox/earcut.hpp>

// Helper to compute signed area of a contour
float signedArea(const std::vector<std::array<float, 2>> &contour) {
    float area = 0.0f;
    size_t n = contour.size();
    for (size_t i = 0; i < n; ++i) {
        const auto &p0 = contour[i];
        const auto &p1 = contour[(i + 1) % n];
        area += (p0[0] * p1[1]) - (p1[0] * p0[1]);
    }
    return area * 0.5f;
}

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
                float y_offset = ypos - h;

                // --- EAR CUT TESSELLATION FOR FRONT/BACK FACES ---
                using Coord = float;
                using Point = std::array<Coord, 2>;
                using Polygon = std::vector<std::vector<Point>>;
                Polygon polygon;
                int start = 0;
                for (size_t cidx = 0; cidx < ch.contourEnds.size(); ++cidx) {
                    int end = ch.contourEnds[cidx];
                    std::vector<Point> pts;
                    for (int i = start; i <= end; ++i)
                        pts.push_back(Point{{ch.contourX[i] + xpos,
                                             ch.contourY[i] + y_offset}});
                    float area = signedArea(pts);
                    // Heuristic: first contour is outer, rest are holes
                    if (cidx == 0) {
                        if (area < 0)
                            std::reverse(pts.begin(), pts.end());
                    } else {
                        if (area > 0)
                            std::reverse(pts.begin(), pts.end());
                    }
                    polygon.push_back(pts);
                    start = end + 1;
                }
                // Flatten polygon for mesh vertices
                std::vector<Point> flatVerts;
                for (const auto &ring : polygon)
                    for (const auto &pt : ring)
                        flatVerts.push_back(pt);
                // Triangulate front face
                plane_quest::render::TextMesh frontMesh;
                for (const auto &pt : flatVerts)
                    frontMesh.vertices.push_back(
                        {glm::vec3(pt[0], pt[1], 0.0f), glm::vec2(0, 0)});
                auto frontIndices = mapbox::earcut<unsigned int>(polygon);
                frontMesh.indices = frontIndices;
                // Triangulate back face
                plane_quest::render::TextMesh backMesh;
                for (const auto &pt : flatVerts)
                    backMesh.vertices.push_back(
                        {glm::vec3(pt[0], pt[1], -depth), glm::vec2(0, 0)});
                auto backIndices = mapbox::earcut<unsigned int>(polygon);
                backMesh.indices = backIndices;
                glm::mat4 model = sharedTransform;
                model = glm::scale(model, glm::vec3(scale));
                model =
                    glm::translate(model, glm::vec3(pen_x / scale, 0.0f, 0.0f));
                glm::vec3 frontColor(0.0f, 1.0f, 1.0f);
                meshRenderer.render(frontMesh, projection, view * model,
                                    frontColor, 0);
                meshRenderer.render(backMesh, projection, view * model,
                                    frontColor, 0);
                // --- CONTOUR-BASED SIDE MESH GENERATION (with offset) ---
                plane_quest::render::TextMesh contourSideMesh;
                start = 0;
                for (size_t cidx = 0; cidx < ch.contourEnds.size(); ++cidx) {
                    int end = ch.contourEnds[cidx];
                    for (int i = start; i <= end; ++i) {
                        int next = (i == end) ? start : i + 1;
                        glm::vec3 v0(ch.contourX[i] + xpos,
                                     ch.contourY[i] + y_offset, 0.0f);
                        glm::vec3 v1(ch.contourX[next] + xpos,
                                     ch.contourY[next] + y_offset, 0.0f);
                        glm::vec3 v2(ch.contourX[next] + xpos,
                                     ch.contourY[next] + y_offset, -depth);
                        glm::vec3 v3(ch.contourX[i] + xpos,
                                     ch.contourY[i] + y_offset, -depth);
                        int base = contourSideMesh.vertices.size();
                        contourSideMesh.vertices.push_back(
                            {v0, glm::vec2(0, 0)});
                        contourSideMesh.vertices.push_back(
                            {v1, glm::vec2(0, 0)});
                        contourSideMesh.vertices.push_back(
                            {v2, glm::vec2(0, 0)});
                        contourSideMesh.vertices.push_back(
                            {v3, glm::vec2(0, 0)});
                        contourSideMesh.indices.push_back(base + 0);
                        contourSideMesh.indices.push_back(base + 1);
                        contourSideMesh.indices.push_back(base + 2);
                        contourSideMesh.indices.push_back(base + 2);
                        contourSideMesh.indices.push_back(base + 3);
                        contourSideMesh.indices.push_back(base + 0);
                    }
                    start = end + 1;
                }
                glm::vec3 sideColor(0.0f, 0.6f, 0.6f);
                meshRenderer.render(contourSideMesh, projection, view * model,
                                    sideColor, 0);

                pen_x += (ch.advance >> 6) * scale;
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