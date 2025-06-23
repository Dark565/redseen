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

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include "font.hh"
#include <iostream>
#include <stdexcept>

namespace plane_quest::render {

class Font::FontImpl {
  public:
    FontImpl(const std::string &fontPath, unsigned int fontSize)
        : fontSize(fontSize) {
        if (FT_Init_FreeType(&ft)) {
            throw std::runtime_error("Failed to initialize FreeType library");
        }
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            FT_Done_FreeType(ft);
            throw std::runtime_error("Failed to load font: " + fontPath);
        }
        FT_Set_Pixel_Sizes(face, 0, fontSize);
        loadGlyphs();
    }

    ~FontImpl() {
        // Clean up textures
        for (const auto &pair : characters) {
            glDeleteTextures(1, &pair.second.textureID);
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    const Character &getCharacter(char c) const {
        auto it = characters.find(c);
        if (it == characters.end()) {
            throw std::out_of_range("Character not loaded in font");
        }
        return it->second;
    }

    unsigned int getFontSize() const { return fontSize; }

    void *getFTFace() const { return face; }

  private:
    void loadGlyphs() {
        glPixelStorei(GL_UNPACK_ALIGNMENT,
                      1); // Disable byte-alignment restriction

        // Load first 128 ASCII characters
        for (unsigned char c = 0; c < 128; c++) {
            // First load the glyph outline
            if (FT_Load_Char(face, c, FT_LOAD_NO_BITMAP)) {
                std::cerr << "Failed to load Glyph outline: " << c << std::endl;
                continue;
            }

            // Get outline data
            FT_GlyphSlot slot = face->glyph;
            FT_Outline &outline = slot->outline;

            // Store outline data
            Character character;
            character.size =
                glm::ivec2(slot->metrics.width >> 6, slot->metrics.height >> 6);
            character.bearing = glm::ivec2(slot->metrics.horiBearingX >> 6,
                                           slot->metrics.horiBearingY >> 6);
            character.advance = slot->advance.x;

            // Copy outline points and tags
            character.contourX.resize(outline.n_points);
            character.contourY.resize(outline.n_points);
            character.contourTags.resize(outline.n_points);
            for (int i = 0; i < outline.n_points; i++) {
                character.contourX[i] = outline.points[i].x /
                                        64.0f; // Convert from 26.6 fixed point
                character.contourY[i] = outline.points[i].y / 64.0f;
                character.contourTags[i] = outline.tags[i];
            }

            // Copy contour end points
            character.contourEnds.resize(outline.n_contours);
            for (int i = 0; i < outline.n_contours; i++) {
                character.contourEnds[i] = outline.contours[i];
            }

            // Now load the bitmap for texture
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "Failed to load Glyph bitmap: " << c << std::endl;
                continue;
            }

            // Generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                         face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                         face->glyph->bitmap.buffer);

            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            character.textureID = texture;
            characters.insert(std::pair<char, Character>(c, character));
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    FT_Library ft;
    FT_Face face;
    unsigned int fontSize;
    std::map<char, Character> characters;
};

Font::Font(const std::string &fontPath, unsigned int fontSize)
    : impl(std::make_unique<FontImpl>(fontPath, fontSize)) {}

Font::~Font() = default;

const Character &Font::getCharacter(char c) const {
    return impl->getCharacter(c);
}

unsigned int Font::getFontSize() const { return impl->getFontSize(); }

void *Font::getFTFace() const { return impl->getFTFace(); }

} // namespace plane_quest::render