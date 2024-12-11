// Bibliotecas externas
#include <memory>
#include <algorithm>
#include <iostream>
#include <glad/glad.h>

// Bibliotecas próprias
#include <window.hpp>
#include <camera.hpp>
#include <GLM/trigonometric.hpp>
#include <shader.hpp>
#include <mesh.hpp>
#include <game.hpp>

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
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

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
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_W)
            game->player.camera.moveForward = true;
        if (key == GLFW_KEY_S)
            game->player.camera.moveBackward = true;
        if (key == GLFW_KEY_A)
            game->player.camera.moveLeft = true;
        if (key == GLFW_KEY_D)
            game->player.camera.moveRight = true;
        if (key == GLFW_KEY_SPACE)
            game->player.camera.position.y *= -1.0f;
    }

    if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_W)
            game->player.camera.moveForward = false;
        if (key == GLFW_KEY_S)
            game->player.camera.moveBackward = false;
        if (key == GLFW_KEY_A)
            game->player.camera.moveLeft = false;
        if (key == GLFW_KEY_D)
            game->player.camera.moveRight = false;
    }
}

void cursorCallback(GLFWwindow* window, double x, double y)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    double dx = x - lastCursorX;
    double dy = y - lastCursorY;

    double sensivity = 0.05;
    game->player.camera.theta -= sensivity * dx;
    game->player.camera.phi += sensivity * dy;

    constexpr double maxPhi = 89.0f;
    game->player.camera.phi = std::clamp(game->player.camera.phi, -maxPhi, maxPhi);
    game->player.camera.hasBeenRotated = true;

    lastCursorX = x;
    lastCursorY = y;
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    // Acho que não pode usar glViewport
    glViewport(0, 0, width, height);
    game->player.camera.updateProjectionMatrix();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        game->player.hasShot = true;
    }
}