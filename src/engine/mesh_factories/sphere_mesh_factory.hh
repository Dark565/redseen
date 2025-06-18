#pragma once

#include "engine/mesh_factory.hh"

namespace plane_quest::engine::mesh_factories {

class Sphere : public MeshFactory {
  public:
    Sphere(float radius = 1.0f, int sectors = 8, int stacks = 8);
    ~Sphere() override = default;

    // Create a sphere mesh with the configured parameters
    std::unique_ptr<render::Mesh> create_mesh() override;

  private:
    float radius;
    int sectors;
    int stacks;
};

} // namespace plane_quest::engine::mesh_factories