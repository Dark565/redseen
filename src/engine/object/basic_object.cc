#include "basic_object.hh"

#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/matrix.hpp>
#include <memory>
#include "engine/engine.hh"
#include "engine/geometry.hh"
#include "render/model.hh"

namespace plane_quest::engine {

BasicObject::BasicObject(const glm::mat4 &transform,
                         const std::shared_ptr<const render::Model> &model)
    : transform(transform), model(model) {}

BasicObject::BasicObject(const Position3f &pos,
                         const std::shared_ptr<const render::Model> &model)
    : BasicObject(glm::translate(glm::mat4(1.0f), pos), model) {}

std::shared_ptr<const render::Model> BasicObject::get_model() const {
    return model;
}

void BasicObject::set_model(const std::shared_ptr<const render::Model> &model) {
    this->model = model;
}

glm::mat4 BasicObject::get_transform() const { return transform; }

void BasicObject::set_transform(const glm::mat4 &transform) {
    this->transform = transform;
}

Position3f BasicObject::get_pos() const { return transform[3]; }

void BasicObject::set_pos(const Position3f &pos) {
    auto tr = get_transform();
    this->transform[3] = glm::vec4(pos, tr[3][3]);
}
bool BasicObject::render(const std::shared_ptr<Engine> &engine) {
    model->render(engine->get_global_mesh_renderer(), get_transform());
}

} // namespace plane_quest::engine