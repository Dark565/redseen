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

#include <memory>

#include "engine/renderer.hh"
#include "engine/event_observer.hh"

namespace redseen::render {
class OpenGLDrawer;
class MeshRenderer;
class Model;
} // namespace redseen::render
namespace redseen::engine {
class Event;

namespace renderers {

struct OpenGLRenderRequest : RenderRequest {
    std::shared_ptr<render::Model> model;
};

class OpenGLRenderer : public engine::Renderer {
    std::shared_ptr<render::OpenGLDrawer> ogl_drawer;
    std::unique_ptr<render::MeshRenderer> mesh_renderer;

  public:
    OpenGLRenderer(std::shared_ptr<Engine>,
                   std::shared_ptr<render::OpenGLDrawer>);

    void init() override;
    void update() override;
    void present() override;

    bool render(const render::Model &, const glm::mat4 &transform,
                const glm::vec3 &lightPos);

    const std::unique_ptr<render::MeshRenderer> &get_mesh_renderer() const;
};

} // namespace renderers
} // namespace redseen::engine
