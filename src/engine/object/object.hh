#pragma once

#include <memory>
#include <string_view>

#include "engine/geometry.hh"

namespace plane_quest::engine {

class Engine;

class Object {
  protected:
    Object() = default;

  public:
    virtual std::string_view get_name() const = 0;
    virtual Position3f get_pos() const = 0;
    virtual void set_pos(const Position3f &pos) = 0;
    virtual bool render(const std::shared_ptr<Engine> &) = 0;

    friend class ObjectManager;
};
} // namespace plane_quest::engine