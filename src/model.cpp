#include "model.h"
#include <iostream>
#include "shader.h"
#include <assimp/Importer.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Model::LoadModel(const std::string& Path)
{
    Assimp::Importer m_importer;
    // Cargamos el archivo de la escena con los modelos para que sea procesado por Assimp
    const aiScene* scene = m_importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {
        std::cerr << "ERROR::ASSIMP:: " << m_importer.GetErrorString() << std::endl;
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // usamos aiMesh de Assimp para recorrer los meshes de la escena
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Model::ProcessMesh(aiMesh* mesh)
{
    // Creamos vector de vertices
    std::vector<float> model_vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
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

// Implementa el método MakeBindless
void Model::MakeBindless()
{
    // Genera la textura y la envía a la GPU
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Configura los parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Carga las texturas con stb_image y las envía a la GPU
    int width, height, nrChannels;
    unsigned char* data = stbi_load("../textures/texture.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Crea un manejador para las bindless textures
    textureHandle = glGetTextureHandleARB(texture);
    glMakeTextureHandleResidentARB(textureHandle);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::DrawBindless()
{
    // Usa el Bindless texture en el shader
    GLint textureLocation = glGetUniformLocation(shader_program->Id, "bindlessTexture");
    glUniformHandleui64ARB(textureLocation, textureHandle);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}

void Model::SetShader(Shader* shader) {
    shader_program = shader;
}