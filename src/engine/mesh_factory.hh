#pragma once

#include "render/mesh.hh"
#include <memory>

namespace plane_quest::engine {

class MeshFactory {
  public:
    virtual ~MeshFactory() = default;

    // Create a mesh with given parameters
    virtual std::unique_ptr<render::Mesh> create_mesh() = 0;
};

} // namespace plane_quest::engine