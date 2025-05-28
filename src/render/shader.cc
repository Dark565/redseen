#include "shader.hh"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);
    compileAndLink(vertexCode.c_str(), fragmentCode.c_str());
}

Shader::Shader(const char *vertexSource, const char *fragmentSource, bool) {
    compileAndLink(vertexSource, fragmentSource);
}

void Shader::compileAndLink(const char *vShaderCode, const char *fShaderCode) {
    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const { glUseProgram(ID); }

std::string Shader::readFile(const std::string &filePath) const {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open shader file: " << filePath
                  << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::checkCompileErrors(GLuint shader, const std::string &type) const {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "Error: Shader compilation failed (" << type << ")\n"
                      << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "Error: Shader program linking failed\n"
                      << infoLog << std::endl;
        }
    }
}
