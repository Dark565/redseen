#include "mesh.hh"
#include <glad/glad.h>

namespace plane_quest::render {

const Mesh::VertexVec &Mesh::get_vertices() const { return vertices; }

const Mesh::IndexVec &Mesh::get_indices() const { return indices; }

const std::size_t Mesh::get_index_count() const { return indices.size(); }

} // namespace plane_quest::render