#ifndef WINDOW_HPP
#define WINDOW_HPP

// Bibliotecas externas
#include <memory>
#include <shader.hpp>
#include <camera.hpp>
#include <game.hpp>
#include <glfw/glfw3.h>

void ErrorCallback(int error, const char* description) {
	std::cerr << "Erro: " << description << std::endl;
}

double lastCursorX = 0.0;
double lastCursorY = 0.0;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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

		if (key == GLFW_KEY_R) {
			game->shaders[SHADER::MODEL] = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
			game->shaders[SHADER::TERRAIN] = Shader("./shaders/terrain.vs", "./shaders/terrain.fs");
			game->shaders[SHADER::SKYBOX] = Shader("./shaders/skybox.vs", "./shaders/skybox.fs");

		}
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

void cursorCallback(GLFWwindow* window, double x, double y) {
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

void windowSizeCallback(GLFWwindow* window, int width, int height) {
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	glViewport(0, 0, width, height);
	game->player.camera.updateProjectionMatrix();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		game->player.hasShot = true;
	}

}

class Window
{
public:
	Window(int width, int height, const char* title) {
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

	bool isOpen() {
		return !glfwWindowShouldClose(window);
	}

	void clear() {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void swapAndPoll() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	GLFWwindow* getWindow() {
		return window;
	}

private:
	GLFWwindow* window;
};

#endif