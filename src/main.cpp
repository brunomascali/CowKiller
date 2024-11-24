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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define MISC_IMPLEMENTATION
#include <misc.hpp>

int main()
{
    Window window(900, 500, "CowKiller");
    auto deltaTime = std::make_shared<float>(0.0f);

    std::vector<Shader> shaders(10);
    shaders[SHADERS::MODEL] = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    shaders[SHADERS::TERRAIN] = Shader("./shaders/terrain.vs", "./shaders/terrain.fs");

    auto camera_pos = glm::vec3(3.5f, 4.0f, -2.0f);
    auto camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    auto camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    CameraFree camera
    {
        camera_pos, 
        camera_target, 
        camera_up
    };
    camera.shareDeltaTime(deltaTime);
    glfwSetWindowUserPointer(window.getWindow(), &camera);

    Model dragon("./assets/spongebob/spongebob.fbx");
    dragon.translate(glm::vec3(10.0f, 4.0f, 10.0f));
    auto terrain = Terrain(64, 64);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (window.isOpen())
    {
        window.clear();
        updateDT(deltaTime);

        dragon.render(shaders[SHADERS::MODEL], camera);
        terrain.render(shaders[SHADERS::TERRAIN], camera);

        window.swapAndPoll();
    }

    glfwTerminate();

    return 0;
}