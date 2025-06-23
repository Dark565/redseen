#ifndef SHADER_HH
#define SHADER_HH

#include <string>

class Shader {
  public:
    // Constructor for loading from files
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    // Constructor for direct source code
    Shader(const char *vertexSource, const char *fragmentSource,
           bool isSourceCode);

    void use() const;
    unsigned int ID;

  private:
    std::string readFile(const std::string &filePath) const;
    void checkCompileErrors(unsigned int shader, const std::string &type) const;
    void compileAndLink(const char *vertexCode, const char *fragmentCode);
};

#endif
