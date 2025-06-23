/*
 *  Copyright (C) 2025 Grzegorz Kociołek (grzegorz.kclk@gmail.com)
 *
 *  This file is a part of RedSeen; a 3D game engine.
 *
 *  RedSeen is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RedSeen is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <exception>
#include <memory>

#include <glm/glm.hpp>

#include "event_observer.hh"

namespace plane_quest::render {
class OpenGLDrawer;
}
namespace plane_quest::engine {

class Model;
class Engine;
class Event;
class EventDispatcher;

/** A polymorphic structure representing API specific render requests */
struct RenderRequest {
    const Model &model;
    glm::mat4 transform;
    glm::vec3 lightPos;

    RenderRequest(const Model &model, const glm::mat4 &transform,
                  const glm::vec3 &lightPos)
        : model(model), transform(transform) {}

    virtual ~RenderRequest() = default;
};

class Renderer : public EventObserver {
  protected:
    std::shared_ptr<Engine> engine;

  public:
    Renderer(std::shared_ptr<Engine> engine);

    virtual ~Renderer();

    virtual void init() = 0;

    bool render(const RenderRequest &);
    ObserverReturnSignal on_event(const Event &) override;
    struct IncompatibleRendererError : public std::logic_error {
        IncompatibleRendererError(const char *msg) : std::logic_error(msg) {}
    };

    static void subscribe_dispatcher(std::weak_ptr<Renderer> _this,
                                     EventDispatcher &disp);

  protected:
    virtual void update() = 0;
    /** By default Renderer iterates over all objects
    and renders them */
    virtual void render();
    virtual void present() = 0;

    friend class Engine;
};

} // namespace plane_quest::engine