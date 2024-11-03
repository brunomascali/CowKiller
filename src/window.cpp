// Bibliotecas externas
#include <memory>
#include <iostream>
#include <glad.h>

// Bibliotecas próprias
#include <window.hpp>
#include <camera.hpp>

void ErrorCallback(int error, const char *description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    CameraFree *camera = static_cast<CameraFree *>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_W)
            camera->moveForward = true;
        if (key == GLFW_KEY_S)
            camera->moveBackward = true;
        if (key == GLFW_KEY_A)
            camera->moveLeft = true;
        if (key == GLFW_KEY_D)
            camera->moveRight = true;
    }

    if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_W)
            camera->moveForward = false;
        if (key == GLFW_KEY_S)
            camera->moveBackward = false;
        if (key == GLFW_KEY_A)
            camera->moveLeft = false;
        if (key == GLFW_KEY_D)
            camera->moveRight = false;
    }
}

Window::Window(int width, int height, const char *title)
    : lastTime(0.0f)
{
    if (!glfwInit())
    {
        glfwTerminate();
        throw std::runtime_error("Erro ao inicializar o GLFW");
    }

    glfwSetErrorCallback(ErrorCallback);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Erro ao criar janela");
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

bool Window::isOpen()
{
    return !glfwWindowShouldClose(window);
}

void Window::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapAndPoll()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

float Window::getDeltaTime()
{
    float currentTime = glfwGetTime();
    float dt = currentTime - lastTime;
    lastTime = currentTime;

    return dt;
}
