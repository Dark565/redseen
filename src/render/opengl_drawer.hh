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

#pragma once

#include "drawer.hh"
#include "texture.hh"
#include <GLFW/glfw3.h>

namespace redseen::render {

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

} // namespace redseen::render
