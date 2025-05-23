#pragma once

#include <vector>
#include <cstdint>
#include <glad/glad.h>

namespace plane_quest::render {

class Texture {
public:
    int width = 0;
    int height = 0;
    GLuint glTex = 0;

    Texture(int w, int h);
    ~Texture();

    void upload(const std::vector<uint32_t>& pixels);
};

} // namespace plane_quest::render
