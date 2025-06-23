#pragma once

#include "drawer.hh"
#include "texture.hh"
#include <GLFW/glfw3.h>

namespace plane_quest::render {

class OpenGLDrawer : public Drawer {
  public:
    OpenGLDrawer(int width, int height, GLFWwindow *window);
    ~OpenGLDrawer() override;

    void clear(float r, float g, float b, float a = 1.0f) override;
    void drawTexture(const Texture &texture, int x, int y, int width,
                     int height);
    virtual void drawTexture(unsigned int texture, int x, int y, int width,
                             int height);
    void present() override;
    void *getWindowHandle() const;

  private:
    int m_width;
    int m_height;
    GLFWwindow *m_window;
    unsigned int m_fbo = 0;
    unsigned int m_rbo = 0;
    unsigned int m_screenQuadVao = 0;
    unsigned int m_screenQuadVbo = 0;
};

} // namespace plane_quest::render
