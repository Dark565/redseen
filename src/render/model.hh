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

#include "mesh_renderer.hh"
#include <glm/glm.hpp>
#include <memory>

namespace plane_quest::render {

class Shader;
class OpenGLMeshHandle;

class Model {
  public:
    Model(std::shared_ptr<OpenGLMeshHandle> mesh,
          std::shared_ptr<Shader> shader = nullptr)
        : mesh_(std::move(mesh)), shader_(std::move(shader)),
          color_(1.0f, 1.0f, 1.0f), textureID_(0), transform_(1.0f) {}

    const std::shared_ptr<OpenGLMeshHandle> &getMeshHandle() const {
        return mesh_;
    }
    const std::shared_ptr<Shader> &getShader() const { return shader_; }

    void setMesh(std::shared_ptr<OpenGLMeshHandle> mesh) { mesh_ = mesh; }
    void setShader(std::shared_ptr<Shader> shader) { shader_ = shader; }

    void setColor(const glm::vec3 &color) { color_ = color; }
    void setTextureID(unsigned int textureID) { textureID_ = textureID; }
    void setTransform(const glm::mat4 &transform) { transform_ = transform; }
    const glm::vec3 &getColor() const { return color_; }
    unsigned int getTextureID() const { return textureID_; }
    const glm::mat4 &getTransform() const { return transform_; }

    // Renders the model using the given renderer and parent/world transform
    void render(MeshRenderer &renderer, const glm::mat4 &projection,
                const glm::mat4 &parentTransform,
                const glm::vec3 &lightPos) const;

  private:
    std::shared_ptr<OpenGLMeshHandle> mesh_;
    std::shared_ptr<Shader> shader_;
    glm::vec3 color_;
    unsigned int textureID_;
    glm::mat4 transform_;
};

} // namespace plane_quest::render
