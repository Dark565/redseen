#include "basic_object.hh"

#include <memory>
#include "engine/geometry.hh"
#include "render/model.hh"

namespace plane_quest::engine {

BasicObject::BasicObject(const Position3f &pos,
                         const std::shared_ptr<const render::Model> &model)
    : pos(pos), model(model) {}

std::shared_ptr<const render::Model> BasicObject::get_model() const {
    return model;
}

void BasicObject::set_model(const std::shared_ptr<const render::Model> &model) {
    this->model = model;
}

void BasicObject::set_pos(const Position3f &pos) { this->pos = pos; }
Position3f BasicObject::get_pos() const { return this->pos; }

bool BasicObject::render(const std::shared_ptr<Engine> &engine) { return true; }

} // namespace plane_quest::engine