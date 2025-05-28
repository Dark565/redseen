#ifndef FONT_HH
#define FONT_HH

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <memory>

namespace plane_quest::render {

struct Character {
    unsigned int textureID;
    glm::ivec2 size;      // Size of glyph
    glm::ivec2 bearing;   // Offset from baseline to left/top of glyph
    unsigned int advance; // Horizontal offset to advance to next glyph
};

class Font {
  public:
    Font(const std::string &fontPath, unsigned int fontSize);
    ~Font();

    // Get character data for a specific character
    const Character &getCharacter(char c) const;
    unsigned int getFontSize() const { return fontSize; }

  private:
    void loadGlyphs();

    FT_Library ft;
    FT_Face face;
    unsigned int fontSize;
    std::map<char, Character> characters;
};

} // namespace plane_quest::render

#endif