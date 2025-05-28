#ifndef TEXT_HH
#define TEXT_HH

#include "font.hh"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace plane_quest::render {

class TextRenderer;

class Text {
  public:
    // Constructor is private, Text objects are created through TextRenderer
    ~Text();

    // Render the text at the specified position
    void render(const std::string &text, float x, float y, float scale,
                const glm::vec3 &color,
                const glm::mat4 &model = glm::mat4(1.0f));

    // Get the font used by this text object
    const Font *getFont() const { return font.get(); }

  private:
    Text(TextRenderer *renderer, const std::shared_ptr<Font> &font);

    TextRenderer *renderer; // Non-owning pointer to the renderer
    std::shared_ptr<Font> font;

    friend class TextRenderer;
};

} // namespace plane_quest::render

#endif
