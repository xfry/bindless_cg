#include "viewport.h"
#include <iostream>

Viewport::Viewport(int width, int height, char* title, const char* modelPath) :
    width(width), height(height), title(title), modelPath(modelPath) { }

void Viewport::Init()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    shader_program = std::make_unique<Shader>("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    model = std::make_unique<Model>(modelPath);
}

void Viewport::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_program->use();

    // Definimos la matriz de modelos
    glm::mat4 model_matx = glm::mat4(1.0f);
    model_matx = glm::translate(model_matx, glm::vec3(0.0f, 0.0f, -1.0f)); // Mueve el modelo hacia la cámara

    // Definimos la matriz de vista
    glm::mat4 view_mtx = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Definimos la matriz de proyección con perspectiva
    glm::mat4 project_mtx = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // Pasamos las matrices al shader
    shader_program->setMat4("model", model_matx);
    shader_program->setMat4("view", view_mtx);
    shader_program->setMat4("projection", project_mtx);

    // Pasamos las posiciones de la luz y la cámara al shader
    shader_program->setVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
    shader_program->setVec3("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));
    shader_program->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader_program->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));

    model->Draw();
}

void Viewport::Run()
{
    while (!glfwWindowShouldClose(window)) {
        // Renderizamos la escena 
        Render();
        // limpiamos los buffers de la ventana
        glfwSwapBuffers(window);
        // Escuche el polling de eventos
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
