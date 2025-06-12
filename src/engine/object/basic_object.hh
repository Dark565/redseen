#pragma once

#include "object.hh"

#include <memory>

#include "render/model.hh"
#include "engine/geometry.hh"

namespace plane_quest::engine {

class Engine;

class BasicObject : public Object {
    glm::mat4 transform;
    std::shared_ptr<const render::Model> model;

  public:
    BasicObject(const glm::mat4 &,
                const std::shared_ptr<const render::Model> &);

    BasicObject(const Position3f &,
                const std::shared_ptr<const render::Model> &);

    std::shared_ptr<const render::Model> get_model() const;
    void set_model(const std::shared_ptr<const render::Model> &model);

    glm::mat4 get_transform() const;
    void set_transform(const glm::mat4 &transform);

    Position3f get_pos() const override;
    void set_pos(const Position3f &pos) override;
    bool render(const std::shared_ptr<Engine> &) override;
};

} // namespace plane_quest::engine