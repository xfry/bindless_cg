#include "obj_loader.h"
#include <tiny_obj_loader.h>
#include <iostream>
#include <stdexcept>

// Cargar modelo OBJ usando tinyobjloader
OBJModel OBJLoader::loadOBJ(const std::string& filepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        throw std::runtime_error("Error al cargar OBJ: " + warn + err);
    }

    OBJModel model;
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            model.vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            model.vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            model.vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (!attrib.normals.empty()) {
                model.normals.push_back(attrib.normals[3 * index.normal_index + 0]);
                model.normals.push_back(attrib.normals[3 * index.normal_index + 1]);
                model.normals.push_back(attrib.normals[3 * index.normal_index + 2]);
            }

            model.indices.push_back(model.indices.size());
        }
    }
    return model;
}

// Configurar buffers de OpenGL para el modelo
void OBJLoader::setupBuffers(OBJModel& model) {
    // Generar VAO (Vertex Array Object)
    glGenVertexArrays(1, &model.VAO);
    glBindVertexArray(model.VAO);

    // Generar y vincular VBO (Vertex Buffer Object) para vértices
    glGenBuffers(1, &model.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(float), model.vertices.data(), GL_STATIC_DRAW);

    // Generar y vincular EBO (Element Buffer Object) para índices
    glGenBuffers(1, &model.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(unsigned int), model.indices.data(), GL_STATIC_DRAW);

    // Configurar atributos del VAO (posición de los vértices)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Desvincular el VAO para evitar modificaciones accidentales
    glBindVertexArray(0);
}
