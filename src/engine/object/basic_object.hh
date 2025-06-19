#pragma once

#include <memory>

#include "object.hh"

namespace plane_quest::engine {

class Model;
class Engine;

/** External objects should derive from this class */
class BasicObject : public Object {
    glm::mat4 transform;
    std::shared_ptr<const Model> model;

  public:
    BasicObject(const glm::mat4 &, std::shared_ptr<const Model>);
    BasicObject(const Position3f &, std::shared_ptr<const Model>);

    std::shared_ptr<const Model> get_model() const;
    void set_model(std::shared_ptr<const Model> model);

    glm::mat4 get_transform() const;
    void set_transform(const glm::mat4 &transform);

    Position3f get_pos() const override;
    void set_pos(const Position3f &pos) override;

    ObjectUpdateResult update(Engine &) override;
    bool render(Engine &) override;
};

} // namespace plane_quest::engine