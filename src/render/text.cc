#include "text.hh"
#include "text_impl.hh" // Include the header file for TextImpl to resolve incomplete type issues

#include <string>
#include <memory>

namespace plane_quest::render {

Text::Text(const std::string &fontPath, unsigned int fontSize)
    : impl(std::make_unique<TextImpl>(fontPath, fontSize)) {}

void Text::renderText(const std::string &text, float x, float y, float scale,
                      const glm::vec3 &color) {
    impl->renderText(text, x, y, scale, color);
}

} // namespace plane_quest::render
