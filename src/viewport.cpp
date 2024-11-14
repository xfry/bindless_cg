#include <iostream>
#include <viewport.h>

Viewport::Viewport(int width, int height, char* title):
    width(width), height(height), title(title) { }

void Viewport::init()
{
    if ( !glfwInit() ) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if ( !window ) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK ) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    shader_program  = std::make_unique<Shader>("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    model           = std::make_unique<Model>("../models/my_model.obj");

}

void Viewport::render()
{
    // Primero limpiamos el buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_program->use();

    // Definimos la matriz de modelos
    glm::mat4 model_matx  = glm::mat4(1.0f);
    // Que la matrix de vista inicie mirando hacia el eje z positivo y que la camara este en el origen
    glm::mat4 view_mtx    = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // Agregamos la matrix de projección con perspectiva
    glm::mat4 project_mtx = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // pasamos las matrices al shader
    shader_program->setMat4("model", model_matx);
    shader_program->setMat4("view", view_mtx);
    shader_program->setMat4("projection", project_mtx);

    model->draw();
}