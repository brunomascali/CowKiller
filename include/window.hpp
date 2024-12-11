#ifndef WINDOW_HPP
#define WINDOW_HPP

// Bibliotecas externas
#include <memory>
#include <shader.hpp>
#include <camera.hpp>
#include <glfw/glfw3.h>

void ErrorCallback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorCallback(GLFWwindow* window, double x, double y);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

static double lastCursorX = 0.0;
static double lastCursorY = 0.0;

class Window
{
public:
    Window(int width, int height, const char *title);
    bool isOpen();
    void clear();
    void swapAndPoll();

    void initCrosshair();
    void drawCrosshair(Shader& shader, CameraFree& camera) const;

    GLFWwindow *getWindow() { return window; }
private:
    GLFWwindow *window;

    GLuint VAO;
};

#endif