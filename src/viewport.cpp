#include "viewport.h"
#include <iostream>

Viewport::Viewport(int width, int height, char* title, const char* modelPath) :
    width(width), height(height), title(title), modelPath(modelPath), firstMouse(true), lastX(width / 2.0f), lastY(height / 2.0f), yaw(-90.0f), pitch(0.0f), mouseButtonPressed(false) { }

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

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    shader_program = std::make_unique<Shader>("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    model = std::make_unique<Model>(modelPath);
    model->SetShader(shader_program.get()); // Set the shader program for the model
}

void Viewport::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_program->use();

    // Definimos la matriz de modelos
    glm::mat4 model_matx = glm::mat4(1.0f);
    model_matx = glm::translate(model_matx, glm::vec3(0.0f, 0.0f, -1.0f)); // Mueve el modelo hacia la cámara

    // Definimos la matriz de vista usando la posición de la cámara
    glm::mat4 view_mtx = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // Definimos la matriz de proyección con perspectiva
    glm::mat4 project_mtx = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // Pasamos las matrices al shader
    shader_program->setMat4("model", model_matx);
    shader_program->setMat4("view", view_mtx);
    shader_program->setMat4("projection", project_mtx);

    // Pasamos las posiciones de la luz y la cámara al shader
    shader_program->setVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
    shader_program->setVec3("viewPos", cameraPos);
    shader_program->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader_program->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));

    model->Draw();
}

void Viewport::Run()
{
    while (!glfwWindowShouldClose(window)) {
        ProcessInput();
        Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Viewport::ProcessInput()
{
    const float cameraSpeed = 0.01f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Viewport::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Agregamos callback para el movimieento del mouse
void Viewport::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    Viewport* viewport = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
    if (!viewport->mouseButtonPressed) {
        return;
    }

    if (viewport->firstMouse)
    {
        viewport->lastX = xpos;
        viewport->lastY = ypos;
        viewport->firstMouse = false;
    }

    float xoffset = xpos - viewport->lastX;
    float yoffset = viewport->lastY - ypos; // y-coordinates go from bottom to top
    viewport->lastX = xpos;
    viewport->lastY = ypos;

    const float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    viewport->yaw += xoffset;
    viewport->pitch += yoffset;

    if (viewport->pitch > 89.0f)
        viewport->pitch = 89.0f;
    if (viewport->pitch < -89.0f)
        viewport->pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(viewport->yaw)) * cos(glm::radians(viewport->pitch));
    front.y = sin(glm::radians(viewport->pitch));
    front.z = sin(glm::radians(viewport->yaw)) * cos(glm::radians(viewport->pitch));
    viewport->cameraFront = glm::normalize(front);
}

void Viewport::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Viewport* viewport = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        viewport->mouseButtonPressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        viewport->mouseButtonPressed = false;
    }
}