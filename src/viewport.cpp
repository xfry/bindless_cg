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