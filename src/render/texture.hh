#pragma once
#include <glad/glad.h>

namespace plane_quest::render {

class Texture {
  public:
    // Construct from raw RGBA8 pixel data
    Texture(const unsigned char *data, int width, int height);
    // Construct from an existing OpenGL texture ID (does not take ownership by
    // default)
    Texture(GLuint existingId, int width, int height, bool ownsTexture = false);
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
    bool m_ownsTexture = true;
};

} // namespace plane_quest::render
