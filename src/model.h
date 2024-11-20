#ifndef __Model_H__
#define __Model_H__

#include <GL/glew.h>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"

class Model {
public:
    Model( const std::string& path );
    void Draw();
    void MakeBindless();
    void DrawBindless();
    void SetShader(Shader* shader); // Ensure this method is declared

private:
    void LoadModel( const std::string& path );
    unsigned int num_vertices;
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh);
    // Declaramos las variables del Vertex Buffer Object y Vertex Array Object
    GLuint vao, vbo;
    // Declaramos las variables para la textura bindless
    GLuint texture;
    GLuint64 textureHandle;
    Shader* shader_program;
};

#endif