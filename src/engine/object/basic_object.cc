#include "basic_object.hh"

#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/matrix.hpp>
#include <memory>
#include "engine/engine.hh"
#include "engine/model.hh"
#include "engine/geometry.hh"
#include "engine/object/object.hh"
#include "render/model.hh"

namespace plane_quest::engine {

BasicObject::BasicObject(const glm::mat4 &transform,
                         std::shared_ptr<const Model> model)
    : transform(transform), model(std::move(model)) {}

BasicObject::BasicObject(const Position3f &pos,
                         std::shared_ptr<const Model> model)
    : BasicObject(glm::translate(glm::mat4(1.0f), pos), std::move(model)) {}

std::shared_ptr<const Model> BasicObject::get_model() const { return model; }

void BasicObject::set_model(std::shared_ptr<const Model> model) {
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

ObjectUpdateResult BasicObject::update(Engine &engine) {
    return ObjectUpdateResult::NORMAL;
}

bool BasicObject::render(Engine &engine) {
#ifdef DEBUG
    std::cerr << "Rendering "
              << abi::__cxa_demangle(typeid(*this).name(), nullptr, 0, nullptr)
              << std::endl;
#endif
    return engine.get_renderer()->render({*get_model(), get_transform()});
}

} // namespace plane_quest::engine