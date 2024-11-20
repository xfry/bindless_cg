#ifndef __Viewport_H__
#define __Viewport_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "model.h"
#include "shader.h"

class Viewport {
public:
    Viewport(int width, int height, char* title);
    void Init();
    void Run();

private:
    void Render();
    void SetupCallbacks();

    int width;
    int height;
    // Punteros Raw de la ventana que se construirá
    char* title;
    GLFWwindow* window;
    // Declaramos los vbos y vao que usaremos para el renderizado de los modelos Bindless
    GLuint vao, vbo;
    // Declaramoso smart pointers para los modelos y shaders
    std::unique_ptr<Model> model;
    std::unique_ptr<Shader> shader_program;
};

#endif