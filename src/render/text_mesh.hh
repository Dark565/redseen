#ifndef TEXT_MESH_HH
#define TEXT_MESH_HH

#include "font.hh"
#include "mesh.hh"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace plane_quest::render {

// A mesh for a string of text, suitable for 3D extrusion or custom rendering
class TextMesh : public Mesh {
  public:
    // Vertices and indices for the mesh
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;

    // Generate a 2D mesh for the given text string (quads per glyph, like
    // current system)
    static std::unique_ptr<TextMesh>
    fromString(const std::string &text, const std::shared_ptr<Font> &font,
               float scale = 1.0f);

    const std::vector<MeshVertex> &getVertices() const override {
        return vertices;
    }
    const std::vector<unsigned int> &getIndices() const override {
        return indices;
    }
};

} // namespace plane_quest::render

#endif
