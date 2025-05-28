#ifndef TEXT_RENDERER_HH
#define TEXT_RENDERER_HH

#include "shader.hh"
#include "font.hh"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace plane_quest::render {

class Text;

class TextRenderer {
  public:
    TextRenderer();
    ~TextRenderer();

    // Create a new Text object with the specified font
    std::unique_ptr<Text> createText(const std::shared_ptr<Font> &font);

    // Set the projection matrix for all text objects
    void setProjection(const glm::mat4 &proj);

    // Get the shader program
    Shader *getShader() const { return shader.get(); }

  private:
    std::unique_ptr<Shader> shader;
    glm::mat4 projection;

    // OpenGL objects for rendering
    unsigned int VAO;
    unsigned int VBO;

    friend class Text;
};

} // namespace plane_quest::render

#endif