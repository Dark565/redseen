#pragma once
#include <glad/glad.h>

namespace plane_quest::render {

class Texture {
  public:
    // Construct from raw RGBA8 pixel data
    Texture(const unsigned char *data, int width, int height);
    ~Texture();
    GLuint id() const { return m_textureId; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    void bind() const;
    void unbind() const;

  private:
    GLuint m_textureId = 0;
    int m_width = 0;
    int m_height = 0;
};

} // namespace plane_quest::render
