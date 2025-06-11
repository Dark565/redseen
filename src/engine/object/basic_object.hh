#pragma once

#include "object.hh"

#include <memory>
#include <string_view>

#include "render/model.hh"
#include "engine/engine.hh"
#include "engine/texture_key.hh"
#include "engine/geometry.hh"

namespace plane_quest::engine {
class BasicObject : public Object {
    Position3f pos;
    std::shared_ptr<const render::Model> model;

  public:
    BasicObject(const Position3f &,
                const std::shared_ptr<const render::Model> &);

    std::shared_ptr<const render::Model> get_model() const;
    void set_model(const std::shared_ptr<const render::Model> &model);
    Position3f get_pos() const override;
    void set_pos(const Position3f &pos) override;
    bool render(const std::shared_ptr<Engine> &) override;
    void on_event(const Event &) override;
};

} // namespace plane_quest::engine