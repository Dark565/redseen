#ifndef FONT_HH
#define FONT_HH

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>

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
    unsigned int getFontSize() const;

  private:
    class FontImpl;
    std::unique_ptr<FontImpl> impl;
};

} // namespace plane_quest::render

#endif