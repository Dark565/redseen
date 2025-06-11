#pragma once

#include <memory>
#include <string_view>
#include "engine/geometry.hh"
#include "engine/engine.hh"
#include "engine/event_observer.hh"

namespace plane_quest::engine {

class Engine;

class Object : public EventObserver {
  public:
    virtual std::string_view get_name() const = 0;
    virtual Position3f get_pos() const = 0;
    virtual void set_pos(const Position3f &pos) = 0;
    virtual bool render(const std::shared_ptr<Engine> &) = 0;
};
} // namespace plane_quest::engine