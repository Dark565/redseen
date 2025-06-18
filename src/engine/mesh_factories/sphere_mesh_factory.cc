#include "sphere_mesh_factory.hh"
#include <cmath>

namespace plane_quest::engine::mesh_factories {

Sphere::Sphere(float radius, int sectors, int stacks)
    : radius(radius), sectors(sectors), stacks(stacks) {}

std::unique_ptr<render::Mesh> Sphere::create_mesh() {
    std::vector<render::MeshVertex> vertices;
    std::vector<unsigned int> indices;

    // Generate vertices
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * M_PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2 * M_PI / sectors;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            render::MeshVertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.texCoord = glm::vec2(static_cast<float>(j) / sectors,
                                        static_cast<float>(i) / stacks);
            vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            // For each sector, add two triangles
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return std::make_unique<render::Mesh>(std::move(vertices),
                                          std::move(indices));
}

} // namespace plane_quest::engine::mesh_factories