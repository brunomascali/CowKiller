#ifndef WINDOW_HPP
#define WINDOW_HPP

// Bibliotecas externas
#include <memory>
#include <glfw/glfw3.h>

void ErrorCallback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorCallback(GLFWwindow* window, double x, double y);
void windowSizeCallback(GLFWwindow* window, int width, int height);

static double lastCursorX = 0.0;
static double lastCursorY = 0.0;

class Window
{
public:
    Window(int width, int height, const char *title);
    bool isOpen();
    void clear();
    void swapAndPoll();

    GLFWwindow *getWindow() { return window; }
private:
    GLFWwindow *window;
};

#endif