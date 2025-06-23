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

namespace redseen::render {

class Texture {
  public:
    // Construct from raw RGBA8 pixel data
    Texture(const unsigned char *data, int width, int height);
    // Construct from an existing OpenGL texture ID (does not take ownership by
    // default)
    Texture(unsigned int existingId, int width, int height,
            bool ownsTexture = false);
    ~Texture();
    unsigned int id() const { return m_textureId; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    void bind() const;
    void unbind() const;

  private:
    unsigned int m_textureId = 0;
    int m_width = 0;
    int m_height = 0;
    bool m_ownsTexture = true;
};

} // namespace redseen::render
