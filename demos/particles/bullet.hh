#pragma once

#include <glm/glm.hpp>

#include "engine/object/object.hh"
#include "engine/object/basic_object.hh"

namespace plane_quest::render {
class Model;
}

namespace plane_quest::engine {
class Engine;
class Model;
} // namespace plane_quest::engine

namespace plane_quest::demos::particles {

class Bullet : public engine::BasicObject {
    glm::vec3 start_pos;
    glm::vec3 velocity;
    glm::vec3 accel;
    float max_distance_sq;

  public:
    Bullet(const glm::vec3 &start_pos,
           std::shared_ptr<const engine::Model> model,
           const glm::vec3 &velocity, float accel = 0.f,
           float max_distance = INFINITY);

    Bullet(const glm::mat4 &transform,
           std::shared_ptr<const engine::Model> model,
           const glm::vec3 &velocity, float accel = 0.f,
           float max_distance = INFINITY);

    engine::ObjectUpdateResult update(engine::Engine &) override;
};

} // namespace plane_quest::demos::particles