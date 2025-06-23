/*
 *  Copyright (C) 2025 Grzegorz Kociołek (grzegorz.kclk@gmail.com)
 *
 *  This file is a part of RedSeen; a 3D game engine.
 *
 *  RedSeen is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RedSeen is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "text.hh"
#include "text_renderer.hh"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace redseen::render {

Text::Text(TextRenderer *renderer, const std::shared_ptr<Font> &font)
    : renderer(renderer), font(font) {}

Text::~Text() = default;

void Text::render(const std::string &text, float x, float y, float scale,
                  const glm::vec3 &color, const glm::mat4 &model) {
    auto *shader = renderer->getShader();
    shader->use();

    // Set uniforms
    glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y,
                color.z);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1,
                       GL_FALSE, glm::value_ptr(renderer->projection));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(renderer->VAO);

    // Render each character
    for (const char &c : text) {
        const Character &ch = font->getCharacter(c);

        float xpos = x + ch.bearing.x * scale;
        float ypos = y + ch.bearing.y * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // Generate vertices for this character
        float vertices[6][4] = {
            {xpos, ypos - h, 0.0f, 1.0f},     // bottom left
            {xpos + w, ypos - h, 1.0f, 1.0f}, // bottom right
            {xpos, ypos, 0.0f, 0.0f},         // top left

            {xpos + w, ypos - h, 1.0f, 1.0f}, // bottom right
            {xpos + w, ypos, 1.0f, 0.0f},     // top right
            {xpos, ypos, 0.0f, 0.0f}          // top left
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        // Update VBO for each character
        glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursor for next glyph
        x += (ch.advance >> 6) * scale;
    }

    // Cleanup
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace redseen::render
