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

namespace redseen::render {

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

uniform vec3 lightPosition;
uniform vec3 meshColor;
uniform sampler2D meshTexture;

void main() {
    vec4 outColor;

    if (textureSize(meshTexture, 0).x > 1) {
        // For textured faces (front/back)
        float alpha = texture(meshTexture, TexCoord).r;
        outColor = vec4(meshColor, alpha);
    } else {
        // For solid faces (sides)
        outColor = vec4(meshColor, 1.0);
    }

    FragColor = outColor;
}
)";

#if 0
constexpr const char *MESH_FRAGMENT_SHADER = R"(

#version 330 core
in vec2 TexCoord;
in vec3 WorldPos;    // World-space position from Vertex Shader

layout(location = 0) out vec4 FragColor;

uniform vec3 lightPosition;
uniform vec3 meshColor;
uniform sampler2D meshTexture;

uniform vec3 lightColor;   // Color of the light source
uniform vec3 ambientColor; // Global ambient light (e.g., vec3(0.1, 0.1, 0.1))
uniform float Kc;          // Constant attenuation factor (usually 1.0)
uniform float Kl;          // Linear attenuation factor
uniform float Kq;          // Quadratic attenuation factor

// You might still have fog inputs here if you want to keep that effect
// in float LinearDepth;
// uniform vec3 fogColor;
// uniform float fogStart;
// uniform float fogEnd;


void main() {
    vec4 baseColor; // This will hold the color from meshColor or meshTexture

    if (textureSize(meshTexture, 0).x > 1) {
        // For textured faces (front/back)
        float alpha = texture(meshTexture, TexCoord).r; // Assuming texture's red channel is alpha
        baseColor = vec4(meshColor, alpha);
    } else {
        // For solid faces (sides)
        baseColor = vec4(meshColor, 1.0);
    }

    // --- SIMPLE DISTANCE-BASED LIGHTING ---

    // 1. Calculate Ambient Light
    vec3 ambient = ambientColor; // The ambient light source color

    // 2. Calculate Light Attenuation
    //    a. Distance from fragment to light source
    float distance = length(lightPosition - WorldPos);

    //    b. Attenuation factor (quadratic model)
    float attenuation = 1.0 / (Kc + Kl * distance + Kq * (distance * distance));

    // 3. Apply Attenuation to Light Color
    // This gives us the light color at this fragment's position, dimmed by distance.
    vec3 attenuatedLight = lightColor * attenuation;

    // 4. Combine with Ambient and Apply to Base Color
    // The base color is multiplied by the combined light intensity.
    vec3 finalLitColor = baseColor.rgb * (ambient + attenuatedLight);

    FragColor = vec4(finalLitColor, baseColor.a);

    // --- END SIMPLE DISTANCE-BASED LIGHTING ---

    // If you still have fog, apply it after lighting:
    // float fogFactor = clamp((LinearDepth - fogStart) / (fogEnd - fogStart), 0.0, 1.0);
    // FragColor = vec4(mix(FragColor.rgb, fogColor, fogFactor), FragColor.a);
}

)";

#endif

} // namespace redseen::render
