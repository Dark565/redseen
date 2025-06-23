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

#include "texture.hh"

#include <glad/glad.h>

namespace redseen::render {

Texture::Texture(const unsigned char *data, int width, int height)
    : m_width(width), m_height(height), m_ownsTexture(true) {
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(GLuint existingId, int width, int height, bool ownsTexture)
    : m_textureId(existingId), m_width(width), m_height(height),
      m_ownsTexture(ownsTexture) {}

Texture::~Texture() {
    if (m_textureId && m_ownsTexture) {
        glDeleteTextures(1, &m_textureId);
    }
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, m_textureId); }

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

} // namespace redseen::render
