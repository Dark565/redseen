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

#ifndef TEXT_SHADERS_HH
#define TEXT_SHADERS_HH

namespace redseen::render {

// Vertex shader for text rendering
constexpr const char *TEXT_VERTEX_SHADER = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;

void main() {
    vec4 pos = projection * model * vec4(vertex.xy, 0.0, 1.0);
    gl_Position = pos;
    TexCoords = vertex.zw;
}
)";

// Fragment shader for text rendering
constexpr const char *TEXT_FRAGMENT_SHADER = R"(
#version 330 core
in vec2 TexCoords;
layout(location = 0) out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
    float alpha = texture(text, TexCoords).r;
    FragColor = vec4(textColor, alpha);
}
)";

} // namespace redseen::render

#endif // TEXT_SHADERS_HH