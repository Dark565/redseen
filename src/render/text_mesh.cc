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

#include "text_mesh.hh"
#include "mesh.hh"
#include "font.hh"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include <glm/gtc/constants.hpp>
#include <vector>

namespace plane_quest::render {

// Callback structure to store decomposed outline
struct OutlineDecomposer {
    std::vector<glm::vec2> points;
    float scale;
    float xpos;
    float ypos;

    void moveTo(const FT_Vector *to) {
        points.push_back(glm::vec2(xpos + to->x * scale, ypos + to->y * scale));
    }

    void lineTo(const FT_Vector *to) {
        points.push_back(glm::vec2(xpos + to->x * scale, ypos + to->y * scale));
    }

    void conicTo(const FT_Vector *control, const FT_Vector *to) {
        points.push_back(glm::vec2(xpos + to->x * scale, ypos + to->y * scale));
    }

    void cubicTo(const FT_Vector *control1, const FT_Vector *control2,
                 const FT_Vector *to) {
        points.push_back(glm::vec2(xpos + to->x * scale, ypos + to->y * scale));
    }
};

// FreeType callback functions
extern "C" {
static int moveToCallback(const FT_Vector *to, void *user) {
    auto decomposer = static_cast<OutlineDecomposer *>(user);
    decomposer->moveTo(to);
    return 0;
}

static int lineToCallback(const FT_Vector *to, void *user) {
    auto decomposer = static_cast<OutlineDecomposer *>(user);
    decomposer->lineTo(to);
    return 0;
}

static int conicToCallback(const FT_Vector *control, const FT_Vector *to,
                           void *user) {
    auto decomposer = static_cast<OutlineDecomposer *>(user);
    decomposer->conicTo(control, to);
    return 0;
}

static int cubicToCallback(const FT_Vector *control1, const FT_Vector *control2,
                           const FT_Vector *to, void *user) {
    auto decomposer = static_cast<OutlineDecomposer *>(user);
    decomposer->cubicTo(control1, control2, to);
    return 0;
}
}

std::unique_ptr<Mesh>
TextMeshFactory::fromString(const std::string &text,
                            const std::shared_ptr<Font> &font, float scale) {
    float pen_x = 0.0f;
    const float depth = -5.0f;

    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;

    // Setup FreeType outline callbacks
    FT_Outline_Funcs funcs;
    funcs.move_to = moveToCallback;
    funcs.line_to = lineToCallback;
    funcs.conic_to = conicToCallback;
    funcs.cubic_to = cubicToCallback;
    funcs.shift = 0;
    funcs.delta = 0;

    for (const char &c : text) {
        const Character &ch = font->getCharacter(c);
        float xpos = pen_x + ch.bearing.x * scale;
        float ypos = ch.bearing.y * scale;

        // Get the glyph outline
        FT_Face face = static_cast<FT_Face>(font->getFTFace());
        if (FT_Load_Char(face, c, FT_LOAD_NO_BITMAP)) {
            continue;
        }

        // Decompose the outline
        OutlineDecomposer decomposer;
        decomposer.scale = scale / 64.0f; // Convert from 26.6 fixed point
        decomposer.xpos = xpos;
        decomposer.ypos = ypos;

        FT_Outline_Decompose(&face->glyph->outline, &funcs, &decomposer);

        // Process the decomposed points
        const auto &points = decomposer.points;
        if (points.size() < 3) {
            pen_x += (ch.advance >> 6) * scale;
            continue;
        }

        // Create vertices
        size_t baseVertex = vertices.size();
        for (const auto &point : points) {
            float u = (point.x - xpos) / (ch.size.x * scale);
            float v = 1.0f - (point.y - (ypos - ch.size.y * scale)) /
                                 (ch.size.y * scale);

            // Front vertex
            vertices.push_back(
                {glm::vec3(point.x, point.y, 0.0f), glm::vec2(u, v)});

            // Back vertex
            vertices.push_back(
                {glm::vec3(point.x, point.y, depth), glm::vec2(u, v)});
        }

        // Generate triangles
        for (size_t i = 1; i < points.size() - 1; i++) {
            // Front face (counter-clockwise)
            indices.push_back(baseVertex);
            indices.push_back(baseVertex + i * 2);
            indices.push_back(baseVertex + (i + 1) * 2);

            // Back face (clockwise)
            indices.push_back(baseVertex + 1);
            indices.push_back(baseVertex + (i + 1) * 2 + 1);
            indices.push_back(baseVertex + i * 2 + 1);
        }

        // Generate side faces
        for (size_t i = 0; i < points.size() - 1; i++) {
            size_t current = baseVertex + i * 2;
            size_t next = baseVertex + ((i + 1) % points.size()) * 2;

            // First triangle
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            // Second triangle
            indices.push_back(next);
            indices.push_back(next + 1);
            indices.push_back(current + 1);
        }

        pen_x += (ch.advance >> 6) * scale;
    }

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}

} // namespace plane_quest::render
