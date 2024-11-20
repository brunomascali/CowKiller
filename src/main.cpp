// Bibliotecas externas
#include <iostream>
#include <format>
#include <vector>
#include <memory>
#include <filesystem>

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
#include <shader_program.hpp>
#include <camera.hpp>
#include <model.hpp>

int main()
{
    float last_time = 0.0f;
    auto delta_time = std::make_shared<float>(0.0f);

    Window window(900, 500, "CowKiller");
    ShaderProgram shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    auto camera_pos = glm::vec3(3.5f, 1.0f, 0.0f);
    auto camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    auto camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    CameraFree camera
    {
        camera_pos, 
        camera_target, 
        camera_up
    };
    camera.set_deltatime(delta_time);

    Model dragon("./assets/spongebob/spongebob.fbx");

    glfwSetWindowUserPointer(window.getWindow(), &camera);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    while (window.isOpen())
    {
        window.clear();
        float current_time = static_cast<float>(glfwGetTime());
        *delta_time = current_time - last_time;
        last_time = current_time;

        glm::mat4 v = camera.getViewMatrix();
        glm::mat4 p = camera.getProjectionMatrix();

        shader.use();
        shader.setMat4("view", v);
        shader.setMat4("projection", p);
        dragon.render(shader);
        
        window.swapAndPoll();
    }

    glfwTerminate();

    return 0;
}