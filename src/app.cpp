#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "obj_loader.h"

// Tamaño de la ventana
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Callback para ajustar el viewport al cambiar el tamaño de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    // Crear ventana de OpenGL
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bindless CG", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicializar GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        return -1;
    }

    // Configurar OpenGL
    glEnable(GL_DEPTH_TEST);

    // Cargar modelo OBJ
    OBJModel model = OBJLoader::loadOBJ("assets/models/your_model.obj");
    OBJLoader::setupBuffers(model);

    while (!glfwWindowShouldClose(window)) {
        // Limpiar pantalla
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Dibujar modelo
        glBindVertexArray(model.VAO);
        glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, 0);

        // Intercambiar buffers y procesar eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpiar recursos
    glDeleteVertexArrays(1, &model.VAO);
    glDeleteBuffers(1, &model.VBO);
    glDeleteBuffers(1, &model.EBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
