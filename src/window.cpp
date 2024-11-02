// Bibliotecas externas
#include <memory>
#include <iostream>
#include <glad.h>

// Bibliotecas próprias
#include <window.hpp>

void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

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
