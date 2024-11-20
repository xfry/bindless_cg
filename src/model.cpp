#include "model.h"
#include <iostream>
#include <assimp/Importer.hpp>

void Model::LoadModel(const std::string& Path)
{
    Assimp::Importer m_importer;
    // Cargamos el archivo de la escena con los modelos para que sea procesado por Assimp
    const aiScene* scene = m_importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {
        std::cerr << "ERROR::ASSIMP:: " << m_importer.GetErrorString() << std::endl;
        return;
    } 
    // Creamos vector de vertices
    std::vector<float> model_vertices;
    // usamos aiMesh de Assimp para recorrer los meshes de la escena
    aiMesh* mesh = scene->mMeshes[0];
    for ( unsigned int i = 0; i < mesh->mNumVertices; i++ ) {
        model_vertices.push_back(mesh->mVertices[i].x);
        model_vertices.push_back(mesh->mVertices[i].y);
        model_vertices.push_back(mesh->mVertices[i].z);
    }

    num_vertices = mesh->mNumVertices;
    // Generamos el Vertex Array Object
    glGenVertexArrays(1, &vao);
    // bindiamos el vao
    glBindVertexArray(vao);
    // Generamos el Vertex Buffer Object
    glGenBuffers(1, &vbo);
    // bindiamos el vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // cargamos los vertices en el vbo
    glBufferData(GL_ARRAY_BUFFER,
                    model_vertices.size() * sizeof(float),
                    &model_vertices[0],
                    GL_STATIC_DRAW );
    // Seteamos los atributos de los vertices
    glVertexAttribPointer( 0, 
                            3, 
                            GL_FLOAT, 
                            GL_FALSE, 
                            3 * sizeof(float), 
                            (void*)0 );
    // Habilitamos el atributo de los vertices
    glEnableVertexAttribArray(0);
}

Model::Model(const std::string& Path)
{
    LoadModel(Path);
}

void Model::Draw()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    //glBindVertexArray(0);
}