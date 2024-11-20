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
    Viewport(int width, int height, char* title, const char* modelPath);
    void Init();
    void Run();
    void ProcessInput();
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

private:
    void Render();
    void SetupCallbacks();

    int width;
    int height;
    // Punteros Raw de la ventana que se construirá
    char* title;
    GLFWwindow* window;
    const char* modelPath;
    // Declaramos los vbos y vao que usaremos para el renderizado de los modelos Bindless
    GLuint vao, vbo;
    // Declaramos smart pointers para los modelos y shaders
    std::unique_ptr<Model> model;
    std::unique_ptr<Shader> shader_program;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Variables for mouse callback
    bool mouseButtonPressed;
    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;
    float yaw = -90.0f;
    float pitch = 0.0f;
};

#endif