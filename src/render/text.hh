#ifndef TEXT_HH
#define TEXT_HH

#include <string>
#include <glm/glm.hpp>
#include <memory>

namespace plane_quest::render {

class TextImpl; // Forward declaration of the implementation class

class Text {
  public:
    Text(const std::string &fontPath, unsigned int fontSize);
    ~Text();

    void renderText(const std::string &text, float x, float y, float scale,
                    const glm::vec3 &color);

  private:
    std::unique_ptr<TextImpl> impl; // Pointer to the implementation class
};

} // namespace plane_quest::render

#endif
