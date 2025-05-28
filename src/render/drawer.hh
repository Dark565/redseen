#pragma once

#include <glad/glad.h> // Include OpenGL types like GLuint

namespace plane_quest::render {

class Drawer {
  public:
    virtual ~Drawer() = default;

    virtual void clear(float r, float g, float b, float a = 1.0f) = 0;
    virtual void present() = 0;
};

} // namespace plane_quest::render
