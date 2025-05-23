#ifndef TEXT_IMPL_HH
#define TEXT_IMPL_HH

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace plane_quest::render {

struct Character {
    unsigned int textureID; // ID handle of the glyph texture
    glm::ivec2 size;        // Size of glyph
    glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
    unsigned int advance;   // Offset to advance to next glyph
};

class TextImpl {
  public:
    TextImpl(const std::string &fontPath, unsigned int fontSize);
    ~TextImpl();

    void renderText(const std::string &text, float x, float y, float scale,
                    const glm::vec3 &color);

  private:
    std::map<char, Character> characters;
    unsigned int VAO, VBO;
    FT_Library ft;
    FT_Face face;

    void loadFont(const std::string &fontPath, unsigned int fontSize);
    void initRenderData();
};

} // namespace plane_quest::render

#endif
