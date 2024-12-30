// Bibliotecas externas
#include <iostream>
#include <format>
#include <vector>
#include <memory>
#include <filesystem>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/mat4x4.hpp>
#include <GLM/vec4.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <GLM/gtc/noise.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Bibliotecas próprias
#include <window.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include <model.hpp>
#include <terrain.hpp>
#include <skybox.hpp>
#include <collision.hpp>
#include <game.hpp>
#include <linalg.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main()
{
	// Isso precisa ser a primeira coisa à ser executa na função main
	Window window(900, 500, "CowKiller");

	Game game;
	glfwSetWindowUserPointer(window.getWindow(), &game);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (window.isOpen())
	{
		window.clear();
		game.mainLoop();
		window.swapAndPoll();
	}

	glfwTerminate();

	return 0;
}