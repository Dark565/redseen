#pragma once

#include "drawer.hh"
#include "texture.hh"
#include <GLFW/glfw3.h>
#include <glad/glad.h> // Replace GLEW with GLAD

namespace plane_quest::render {

class TextureDrawer : public Drawer {
  public:
    TextureDrawer(int width, int height, GLFWwindow *window);
    ~TextureDrawer() override;

    void clear(float r, float g, float b, float a = 1.0f) override;
    void drawTexture(GLuint texture, int x, int y, int width,
                     int height) override;
    void present() override;
    GLFWwindow *getWindowHandle() const;

  private:
    int m_width;
    int m_height;
    GLFWwindow *m_window;
    GLuint m_fbo = 0;
    GLuint m_rbo = 0;
    GLuint m_screenQuadVao = 0;
    GLuint m_screenQuadVbo = 0;
};

} // namespace plane_quest::render
