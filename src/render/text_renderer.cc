#include "text_renderer.hh"
#include "text.hh"
#include "text_shaders.hh"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace plane_quest::render {

TextRenderer::TextRenderer() {
    // Create shader from embedded code
    shader = std::make_unique<Shader>(TEXT_VERTEX_SHADER, TEXT_FRAGMENT_SHADER,
                                      true);

    // Set up default projection
    projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

    // Initialize rendering data
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr,
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

std::unique_ptr<Text>
TextRenderer::createText(const std::shared_ptr<Font> &font) {
    return std::unique_ptr<Text>(new Text(this, font));
}

void TextRenderer::setProjection(const glm::mat4 &proj) { projection = proj; }

} // namespace plane_quest::render