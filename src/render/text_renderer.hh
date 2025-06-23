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

#pragma once

#include "shader.hh"
#include "font.hh"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace redseen::render {

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

} // namespace redseen::render