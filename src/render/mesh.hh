#ifndef MESH_HH
#define MESH_HH

#include <concepts>
#include <glm/glm.hpp>
#include <type_traits>
#include <vector>

namespace plane_quest::render {

struct MeshVertex {
    glm::vec3 position;
    glm::vec2 texCoord;
};

class Mesh {
  public:
    using VertexVec = std::vector<MeshVertex>;
    using IndexVec = std::vector<unsigned int>;

  private:
    VertexVec vertices;
    IndexVec indices;

  public:
    template <class V, class I>
        requires std::same_as<std::remove_cvref_t<V>, VertexVec> &&
                     std::same_as<std::remove_cvref_t<I>, IndexVec>
    Mesh(V &&vertices, I &&indices)
        : vertices(std::forward<V>(vertices)),
          indices(std::forward<I>(indices)) {}

    const VertexVec &get_vertices() const;
    const IndexVec &get_indices() const;
    const std::size_t get_index_count() const;
};

} // namespace plane_quest::render

#endif
