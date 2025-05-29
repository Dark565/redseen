#include "text_mesh.hh"
#include "mesh.hh"
#include "font.hh"

namespace plane_quest::render {

std::unique_ptr<TextMesh>
TextMesh::fromString(const std::string &text, const std::shared_ptr<Font> &font,
                     float scale) {
    auto mesh = std::make_unique<TextMesh>();
    float pen_x = 0.0f;
    for (const char &c : text) {
        const Character &ch = font->getCharacter(c);
        float xpos = pen_x + ch.bearing.x * scale;
        float ypos = ch.bearing.y * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        // 4 vertices per quad
        size_t base = mesh->vertices.size();
        mesh->vertices.push_back({glm::vec3(xpos, ypos - h, 0.0f),
                                  glm::vec2(0.0f, 1.0f)}); // bottom left
        mesh->vertices.push_back({glm::vec3(xpos + w, ypos - h, 0.0f),
                                  glm::vec2(1.0f, 1.0f)}); // bottom right
        mesh->vertices.push_back({glm::vec3(xpos + w, ypos, 0.0f),
                                  glm::vec2(1.0f, 0.0f)}); // top right
        mesh->vertices.push_back(
            {glm::vec3(xpos, ypos, 0.0f), glm::vec2(0.0f, 0.0f)}); // top left
        // 2 triangles per quad
        mesh->indices.push_back(base + 0);
        mesh->indices.push_back(base + 1);
        mesh->indices.push_back(base + 2);
        mesh->indices.push_back(base + 2);
        mesh->indices.push_back(base + 3);
        mesh->indices.push_back(base + 0);
        pen_x += (ch.advance >> 6) * scale;
    }
    return mesh;
}

} // namespace plane_quest::render
