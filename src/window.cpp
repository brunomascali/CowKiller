// Bibliotecas externas
#include <memory>
#include <algorithm>
#include <iostream>
#include <glad/glad.h>

// Bibliotecas próprias
#include <window.hpp>
#include <camera.hpp>
#include <GLM/trigonometric.hpp>

extern double lastCursorX;
extern double lastCursorY;

Window::Window(int width, int height, const char *title)
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    lastCursorX = width / 2.0;
    lastCursorY = height / 2.0;
}

bool Window::isOpen()
{
    return !glfwWindowShouldClose(window);
}

void Window::clear()
{
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swapAndPoll()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    CameraFree* camera = static_cast<CameraFree*>(glfwGetWindowUserPointer(window));

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

void cursorCallback(GLFWwindow* window, double x, double y)
{
    CameraFree* camera = static_cast<CameraFree*>(glfwGetWindowUserPointer(window));

    double dx = x - lastCursorX;
    double dy = y - lastCursorY;

    double sensivity = 0.05;
    camera->theta -= sensivity * dx;
    camera->phi += sensivity * dy;

    constexpr double maxPhi = 89.0f;
    camera->phi = std::clamp(camera->phi, -maxPhi, maxPhi);
    camera->hasBeenRotated = true;

    lastCursorX = x;
    lastCursorY = y;
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    CameraFree* camera = static_cast<CameraFree*>(glfwGetWindowUserPointer(window));
    // Acho que não pode usar glViewport
    glViewport(0, 0, width, height);
    camera->updateProjectionMatrix();
}
