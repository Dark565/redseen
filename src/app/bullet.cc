#include "bullet.hh"
#include <glm/ext/scalar_constants.hpp>
#ifdef DEBUG
#include <iostream>
#include <array>
#endif

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
    auto eps_vel = velocity + glm::epsilon<float>();
    this->accel = eps_vel * accel / glm::sqrt(glm::dot(eps_vel, eps_vel));
}

Bullet::Bullet(const glm::mat4 &transform,
               std::shared_ptr<const engine::Model> model,
               const glm::vec3 &velocity, float accel, float max_distance)
    : start_pos(transform[3]), engine::BasicObject(transform, std::move(model)),
      velocity(velocity), max_distance_sq(max_distance * max_distance) {
    auto eps_vel = velocity + glm::epsilon<float>();
    this->accel = eps_vel * accel / glm::sqrt(glm::dot(eps_vel, eps_vel));
}

engine::ObjectUpdateResult Bullet::update(engine::Engine &engine) {
    auto new_pos = get_pos() + velocity;
    set_pos(new_pos);

#ifdef DEBUG
    std::cerr << "-- Bullet transform: --";
    auto tr = get_transform();
    for (auto x : reinterpret_cast<std::array<float, 4 * 4> &>(tr)) {
        std::cerr << x << ", ";
    }
    std::cerr << std::endl;
#endif

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