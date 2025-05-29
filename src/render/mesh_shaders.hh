#ifndef MESH_SHADERS_HH
#define MESH_SHADERS_HH

namespace plane_quest::render {

constexpr const char *MESH_VERTEX_SHADER = R"(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoord;

void main() {
    gl_Position = projection * model * vec4(position, 1.0);
    TexCoord = texCoord;
}
)";

constexpr const char *MESH_FRAGMENT_SHADER = R"(
#version 330 core
in vec2 TexCoord;
layout(location = 0) out vec4 FragColor;

uniform vec3 meshColor;
uniform sampler2D meshTexture;

void main() {
    if (textureSize(meshTexture, 0).x > 1) {
        // For textured faces (front/back)
        float alpha = texture(meshTexture, TexCoord).r;
        FragColor = vec4(meshColor, alpha);
    } else {
        // For solid faces (sides)
        FragColor = vec4(meshColor, 1.0);
    }
}
)";

} // namespace plane_quest::render

#endif
