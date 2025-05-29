#ifndef MESH_HH
#define MESH_HH

#include <glm/glm.hpp>
#include <vector>

namespace plane_quest::render {

struct MeshVertex {
    glm::vec3 position;
    glm::vec2 texCoord;
};

class Mesh {
  public:
    virtual ~Mesh() = default;
    virtual const std::vector<MeshVertex> &getVertices() const = 0;
    virtual const std::vector<unsigned int> &getIndices() const = 0;
};

} // namespace plane_quest::render

#endif
