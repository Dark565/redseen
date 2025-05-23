#pragma once

#include <string>
#include <glad/glad.h>

class Shader {
  public:
    GLuint ID;

    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    void use() const;

  private:
    std::string readFile(const std::string &filePath) const;
    void checkCompileErrors(GLuint shader, const std::string &type) const;
};
