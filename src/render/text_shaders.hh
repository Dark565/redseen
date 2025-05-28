#ifndef TEXT_SHADERS_HH
#define TEXT_SHADERS_HH

namespace plane_quest::render {

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

} // namespace plane_quest::render

#endif // TEXT_SHADERS_HH