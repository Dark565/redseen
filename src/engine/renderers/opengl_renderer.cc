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

#include "opengl_renderer.hh"

#include "engine/engine.hh"
#include "engine/renderer.hh"
#include "render/opengl_drawer.hh"
#include "render/mesh_renderer.hh"
#include "render/model.hh"

#include <glad/glad.h>

namespace redseen::engine::renderers {

OpenGLRenderer::OpenGLRenderer(std::shared_ptr<Engine> engine,
                               std::shared_ptr<render::OpenGLDrawer> ogl_drawer)
    : Renderer(engine), ogl_drawer(ogl_drawer),
      mesh_renderer(std::make_unique<render::MeshRenderer>()) {}

void OpenGLRenderer::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderer::update() { ogl_drawer->clear(0, 0, 0); }

void OpenGLRenderer::present() { ogl_drawer->present(); }

bool OpenGLRenderer::render(const render::Model &model,
                            const glm::mat4 &transform,
                            const glm::vec3 &lightPos) {

    model.render(*mesh_renderer,
                 engine->get_player_camera().getProjectionMatrix(), transform,
                 lightPos);

    return true;
}

const std::unique_ptr<render::MeshRenderer> &
OpenGLRenderer::get_mesh_renderer() const {
    return mesh_renderer;
}

} // namespace redseen::engine::renderers
