#include "bullet.hh"

#include <memory>
#include <glm/glm.hpp>

#include "engine/object/object.hh"
#include "engine/object/basic_object.hh"

namespace plane_quest::app {

Bullet::Bullet(const glm::vec3 &start_pos,
               std::shared_ptr<const engine::Model> model,
               const glm::vec3 &velocity, float accel, float max_distance)
    : start_pos(start_pos), engine::BasicObject(start_pos, std::move(model)),
      velocity(velocity), max_distance_sq(max_distance * max_distance) {
    this->accel = velocity * accel / glm::sqrt(glm::dot(velocity, velocity));
}

Bullet::Bullet(const glm::mat4 &transform,
               std::shared_ptr<const engine::Model> model,
               const glm::vec3 &velocity, float accel, float max_distance)
    : start_pos(transform[3]), engine::BasicObject(transform, std::move(model)),
      velocity(velocity), max_distance_sq(max_distance * max_distance) {
    this->accel = velocity * accel / glm::sqrt(glm::dot(velocity, velocity));
}

engine::ObjectUpdateResult Bullet::update(engine::Engine &engine) {
    auto new_pos = get_pos() + velocity;
    set_pos(new_pos);
    velocity += accel;

    auto dist_vec = new_pos - start_pos;
    auto distance = glm::dot(dist_vec, dist_vec);

    if (distance >= max_distance_sq) {
        velocity = {0, 0, 0};
        accel = {0, 0, 0};
    }

    return engine::ObjectUpdateResult::NORMAL;
}

} // namespace plane_quest::app