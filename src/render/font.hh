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

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace redseen::render {

struct Character {
    unsigned int textureID;
    glm::ivec2 size;      // Size of glyph
    glm::ivec2 bearing;   // Offset from baseline to left/top of glyph
    unsigned int advance; // Horizontal offset to advance to next glyph

    // Outline data
    std::vector<float>
        contourX; // X coordinates of points defining the glyph outline
    std::vector<float>
        contourY; // Y coordinates of points defining the glyph outline
    std::vector<unsigned char>
        contourTags;              // FT_CURVE_TAG values for each point
    std::vector<int> contourEnds; // Indices where contours end
};

class Font {
  public:
    Font(const std::string &fontPath, unsigned int fontSize);
    ~Font();

    // Get character data for a specific character
    const Character &getCharacter(char c) const;
    unsigned int getFontSize() const;
    void *getFTFace() const; // Returns the FreeType face object

  private:
    class FontImpl;
    std::unique_ptr<FontImpl> impl;
};

} // namespace redseen::render