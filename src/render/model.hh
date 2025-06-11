#pragma once

#include "mesh.hh"
#include "mesh_renderer.hh"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace plane_quest::render {

class Shader; // Forward declaration

class Model {
public:
    Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader)
        : mesh_(std::move(mesh)), shader_(std::move(shader)), color_(1.0f, 1.0f, 1.0f), textureID_(0), transform_(1.0f) {}

    const Mesh& getMesh() const { return *mesh_; }
    const Shader& getShader() const { return *shader_; }

    void setColor(const glm::vec3& color) { color_ = color; }
    void setTextureID(unsigned int textureID) { textureID_ = textureID; }
    void setTransform(const glm::mat4& transform) { transform_ = transform; }
    const glm::vec3& getColor() const { return color_; }
    unsigned int getTextureID() const { return textureID_; }
    const glm::mat4& getTransform() const { return transform_; }

    // Renders the model using the given renderer and parent/world transform
    void render(MeshRenderer& renderer, const glm::mat4& projection, const glm::mat4& parentTransform = glm::mat4(1.0f)) const;

private:
    std::shared_ptr<Mesh> mesh_;
    std::shared_ptr<Shader> shader_;
    glm::vec3 color_;
    unsigned int textureID_;
    glm::mat4 transform_;
};

} // namespace plane_quest::render
