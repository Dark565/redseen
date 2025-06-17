#pragma once

#include <memory>
#include <string_view>

#include "engine/geometry.hh"

namespace plane_quest::engine {

class Engine;

enum class ObjectUpdateResult {
    /** Normal update */
    NORMAL,
    /** Destroy the object */
    DESTROY
};

class Object {
  protected:
    Object() = default;

  public:
    virtual std::string_view get_name() const = 0;
    virtual Position3f get_pos() const = 0;
    virtual void set_pos(const Position3f &pos) = 0;
    /** Update the state of the object.
    Called by the ObjectManager on each TICK */
    virtual ObjectUpdateResult update(const std::shared_ptr<Engine> &) = 0;

    /** Render the object. Called by Renderer. */
    virtual bool render(const std::shared_ptr<Engine> &) = 0;

    friend class ObjectManager;
};
} // namespace plane_quest::engine