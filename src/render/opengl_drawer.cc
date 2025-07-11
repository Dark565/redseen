/*
 *  Copyright (C) 2025 Grzegorz Kociołek (grzegorz.kclk@gmail.com)
 *
 *  This file is a part of RedSeen; a 3D game engine.
 *
 *  RedSeen is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RedSeen is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdexcept>

#include <glad/glad.h>

#include "opengl_drawer.hh"
#include "texture.hh"

namespace redseen::render {

OpenGLDrawer::OpenGLDrawer(int width, int height, GLFWwindow *window)
    : m_width(width), m_height(height), m_window(window) {
    // Generate and bind a framebuffer object (FBO)
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Generate a renderbuffer object (RBO) for depth and stencil attachment
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width,
                          m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, m_rbo);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer is not complete");
    }

    // Generate a VAO and VBO for rendering a screen quad
    glGenVertexArrays(1, &m_screenQuadVao);
    glGenBuffers(1, &m_screenQuadVbo);

    glBindVertexArray(m_screenQuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_screenQuadVbo);

    float quadVertices[] = {// positions   // texCoords
                            -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                            0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                            -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                            1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLDrawer::~OpenGLDrawer() {
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_rbo);
    glDeleteVertexArrays(1, &m_screenQuadVao);
    glDeleteBuffers(1, &m_screenQuadVbo);
}

void OpenGLDrawer::clear(float r, float g, float b, float a) {
    // Clear the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLDrawer::drawTexture(const Texture &texture, int x, int y, int width,
                               int height) {
    // Ensure we're drawing to the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture.id());

    // Update the quad vertices with the given position and size
    float quadVertices[] = {
        // positions        // texCoords
        static_cast<float>(x),
        static_cast<float>(y + height),
        0.0f,
        1.0f, // top-left
        static_cast<float>(x),
        static_cast<float>(y),
        0.0f,
        0.0f, // bottom-left
        static_cast<float>(x + width),
        static_cast<float>(y),
        1.0f,
        0.0f, // bottom-right

        static_cast<float>(x),
        static_cast<float>(y + height),
        0.0f,
        1.0f, // top-left
        static_cast<float>(x + width),
        static_cast<float>(y),
        1.0f,
        0.0f, // bottom-right
        static_cast<float>(x + width),
        static_cast<float>(y + height),
        1.0f,
        1.0f // top-right
    };

    // Bind the VAO and update the VBO with the new vertices
    glBindVertexArray(m_screenQuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_screenQuadVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);

    // Draw the quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Unbind everything
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLDrawer::drawTexture(GLuint texture, int x, int y, int width,
                               int height) {
    // Ensure we're drawing to the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set up quad vertices and draw as in the other drawTexture method
    float quadVertices[] = {static_cast<float>(x),
                            static_cast<float>(y + height),
                            0.0f,
                            1.0f,
                            static_cast<float>(x),
                            static_cast<float>(y),
                            0.0f,
                            0.0f,
                            static_cast<float>(x + width),
                            static_cast<float>(y),
                            1.0f,
                            0.0f,
                            static_cast<float>(x),
                            static_cast<float>(y + height),
                            0.0f,
                            1.0f,
                            static_cast<float>(x + width),
                            static_cast<float>(y),
                            1.0f,
                            0.0f,
                            static_cast<float>(x + width),
                            static_cast<float>(y + height),
                            1.0f,
                            1.0f};

    glBindVertexArray(m_screenQuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_screenQuadVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLDrawer::present() { glfwSwapBuffers(m_window); }

void *OpenGLDrawer::getWindowHandle() const { return m_window; }

} // namespace redseen::render
