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

#include "opengl_model.hh"
#include "engine/renderer.hh"
#include "engine/renderers/opengl_renderer.hh"

namespace redseen::engine::model {
OpenGLModel::OpenGLModel(std::shared_ptr<render::Model> model) : model(model) {}

bool OpenGLModel::render(Renderer &renderer, const RenderRequest &req,
                         const glm::mat4 &view) const {
    auto ogl_renderer = dynamic_cast<renderers::OpenGLRenderer *>(&renderer);
    if (ogl_renderer == nullptr)
        throw Renderer::IncompatibleRendererError(
            "OpenGLModel can only be rendered with OpenGLRenderer");

    return ogl_renderer->render(*model, view * req.transform, req.lightPos);
}

} // namespace redseen::engine::model