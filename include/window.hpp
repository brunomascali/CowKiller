#ifndef WINDOW_HPP
#define WINDOW_HPP

// Bibliotecas externas
#include <memory>
#include <glfw3.h>

void ErrorCallback(int error, const char* description);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

class Window
{
public:
    Window(int width, int height, const char *title);
    bool isOpen();
    void clear(float r, float g, float b, float a);
    void swapAndPoll();

    GLFWwindow *getWindow() { return window; }
    float getDeltaTime();

private:
    GLFWwindow *window;

    float lastTime;
};

#endif