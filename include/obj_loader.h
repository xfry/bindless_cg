#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <vector>
#include <string>
#include <GL/glew.h>

struct OBJModel {
    std::vector<float> vertices;     // Coordenadas de vértices
    std::vector<float> normals;      // Normales
    std::vector<unsigned int> indices; // Índices
    GLuint VAO, VBO, EBO;            // Buffers de OpenGL
};

class OBJLoader {
public:
    static OBJModel loadOBJ(const std::string& filepath);
    static void setupBuffers(OBJModel& model);
};

#endif // OBJ_LOADER_H
