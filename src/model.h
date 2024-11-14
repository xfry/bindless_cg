#ifndef __Model_H__
#define __Model_H__

#include <GL/glew.h>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    Model( const std::string& path );
    void draw();

private:
    void LoadModel( const std::string& path );
    unsigned int num_vertices;
    GLint vao, vbo;
};

#endif