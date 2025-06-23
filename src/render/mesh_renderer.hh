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

#include <glm/glm.hpp>
#include <memory>

#include "render/opengl_mesh_handle.hh"
#include "shader.hh"

namespace redseen::render {

class MeshRenderer {
  public:
    MeshRenderer();
    ~MeshRenderer();

    void render(const OpenGLMeshHandle &mesh, const glm::mat4 &projection,
                const glm::mat4 &model, const glm::vec3 &color,
                unsigned int textureID, const glm::vec3 &lightPosition);

  private:
    std::unique_ptr<Shader> shader;
};

} // namespace redseen::render