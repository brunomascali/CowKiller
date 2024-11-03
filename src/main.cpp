// Bibliotecas externas
#include <iostream>
#include <format>
#include <glad.h>
#include <glfw3.h>
#include <GLM/mat4x4.hpp>
#include <GLM/vec4.hpp>
#include <GLM/gtx/string_cast.hpp>

// Bibliotecas próprias
#include <window.hpp>
#include <shader_program.hpp>
#include <camera.hpp>

GLuint buildTriangle()
{
    GLfloat vertices[] = {
        -0.5f,
        0.5f,
        0.0f,
        1.0f,
        0.0f,
        0.75f,
        0.0f,
        1.0f,
        0.5f,
        0.5f,
        0.0f,
        1.0f,
    };

    GLuint indices[] = {
        0, 1, 2};
    GLuint VAO = 0, VBO_vertices = 0, VBO_indices = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_vertices);
    glGenBuffers(1, &VBO_indices);

    glBindVertexArray(VAO);

    // Aloca memória e popula o buffer com os vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    // Aloca memória e popula o buffer com os indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

    GLuint location = 0;
    GLuint dimensions = 4;
    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);

    // Desligar VAO
    glBindVertexArray(0);

    return VAO;
}

int main()
{
    Window window(600, 600, "Universe");
    ShaderProgram shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    CameraFree camera{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 100.0f, 3.1415f / 3.0f};

    GLuint t = buildTriangle();

    glfwSetWindowUserPointer(window.getWindow(), &camera);
    glfwSetKeyCallback(window.getWindow(), keyCallback);

    glUseProgram(shader.getProgramID());

    while (window.isOpen())
    {
        window.clear(0.0f, 0.0f, 0.0f, 0.0f);
        float dt = window.getDeltaTime();

        auto m = glm::mat4(1);

        camera.updateCameraPosition(dt);
        glm::mat4 v = camera.getViewMatrix();
        glm::mat4 p = camera.getProjectionMatrix();
        shader.setMat4("model", m);
        shader.setMat4("view", v);
        shader.setMat4("projection", p);
        glBindVertexArray(t);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        window.swapAndPoll();
    }

    glfwTerminate();

    return 0;
}