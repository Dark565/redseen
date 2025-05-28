#include "texture.hh"

namespace plane_quest::render {

Texture::Texture(const unsigned char *data, int width, int height)
    : m_width(width), m_height(height) {
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    if (m_textureId) {
        glDeleteTextures(1, &m_textureId);
    }
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, m_textureId); }

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

} // namespace plane_quest::render
