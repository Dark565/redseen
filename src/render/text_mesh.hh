#ifndef TEXT_MESH_HH
#define TEXT_MESH_HH

#include "font.hh"
#include "mesh.hh"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace plane_quest::render {

// TODO: Integrate it with engine's MeshFactory
class TextMeshFactory {
  public:
    // Generate a 2D mesh for the given text string (quads per glyph, like
    // current system)
    std::unique_ptr<Mesh> fromString(const std::string &text,
                                     const std::shared_ptr<Font> &font,
                                     float scale = 1.0f);
};

} // namespace plane_quest::render

#endif
