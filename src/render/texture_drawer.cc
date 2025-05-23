#include "texture_drawer.hh"
#include <stdexcept>

namespace plane_quest::render {

TextureDrawer::TextureDrawer(int width, int height, GLFWwindow *window)
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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
}

TextureDrawer::~TextureDrawer() {
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_rbo);
    glDeleteVertexArrays(1, &m_screenQuadVao);
    glDeleteBuffers(1, &m_screenQuadVbo);
}

void TextureDrawer::clear(float r, float g, float b, float a) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureDrawer::drawTexture(const Texture &tex, int x, int y, int w,
                                int h) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, tex.glTex);

    // Render the quad (stub: actual shader and transformation setup needed)
    glBindVertexArray(m_screenQuadVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureDrawer::present() {
    glfwSwapBuffers(m_window); // Swap the front and back buffers
}

GLFWwindow *TextureDrawer::getWindowHandle() const { return m_window; }

} // namespace plane_quest::render
