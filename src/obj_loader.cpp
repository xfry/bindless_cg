#include "obj_loader.h"
#include <tiny_obj_loader.h>
#include <iostream>
#include <stdexcept>

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

void OBJLoader::setupBuffers(OBJModel& model) {
    glGenVertexArrays(1, &model.VAO);
    glGenBuffers(1, &model.VBO);
    glGenBuffers(1, &model.EBO);

    glBindVertexArray(model.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(float), model.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(unsigned int), model.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
::contentReference[oaicite:0]{index=0}
 
