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
