#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
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

  private:
    void loadGlyphs() {
        glPixelStorei(GL_UNPACK_ALIGNMENT,
                      1); // Disable byte-alignment restriction

        // Load first 128 ASCII characters
        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "Failed to load Glyph: " << c << std::endl;
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

            // Store character
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)};
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

} // namespace plane_quest::render