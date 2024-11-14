#ifndef __Shader_H__
#define __Shader_H__

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {

public:
    Shader( const char* vertexPath, const char* fragmentPath );
    void use();
    void setMat4( const std::string& name, const glm::mat4& value );
private:
    unsigned int Id;
    std::string loadShader( const char* path );
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif