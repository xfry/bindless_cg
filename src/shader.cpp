#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = loadShader(vertexPath);
    std::string fragmentCode = loadShader(fragmentPath);

    // Tomamos el codigo de los shaders
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compilamos el vertex shaders
    GLuint vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Compilamos el fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Creamos el programa con los fragment y los vertex shaders
    Id = glCreateProgram();
    glAttachShader(Id, vertex);
    glAttachShader(Id, fragment);
    glLinkProgram(Id);
    checkCompileErrors(Id, "PROGRAM");

    // Borramos los shaders ya que no los necesitamos despuúes de haberlos linkeado
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(Id);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

std::string Shader::loadShader(const char* path) {
    std::ifstream shaderFile;
    std::stringstream shaderStream;

    shaderFile.open(path);
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
}
