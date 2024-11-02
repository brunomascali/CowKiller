#ifndef WINDOW_HPP
#define WINDOW_HPP

// Bibliotecas externas
#include <memory>
#include <glfw3.h>

class Window
{
public:
    Window(int width, int height, const char *title);
    bool isOpen();
    void clear(float r, float g, float b, float a);
    void swapAndPoll();

    GLFWwindow *getWindow() { return window; }

private:
    GLFWwindow *window;
};

#endif