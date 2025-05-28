#include "text.hh"
#include "text_impl.hh"

#include <string>

namespace plane_quest::render {

Text::Text(const std::string &fontPath, unsigned int fontSize)
    : impl(std::make_unique<TextImpl>(fontPath, fontSize)) {}

Text::~Text() = default;

void Text::renderText(const std::string &text, float x, float y, float scale,
                      const glm::vec3 &color) {
    impl->renderText(text, x, y, scale, color);
}

} // namespace plane_quest::render
