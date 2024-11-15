// Bibliotecas externas
#include <iostream>
#include <format>
#include <vector>
#include <memory>

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

//GLuint foo()
//{
//    Assimp::Importer importer;
//    const aiScene *scene = importer.ReadFile("Dragon 2.5_dae.dae",
//                                             aiProcess_CalcTangentSpace |
//                                                 aiProcess_Triangulate |
//                                                 aiProcess_JoinIdenticalVertices |
//                                                 aiProcess_SortByPType);
//    if (scene == nullptr)
//    {
//        throw std::runtime_error("Erro ao importar .dae");
//    }
//
//    std::vector<float> vertices;
//    std::vector<unsigned int> indices;
//
//    for (int i = 2; i < 3; i++) {
//     for (int i = 0; i < scene->mNumMeshes; i++) {
//        const aiMesh* mesh = scene->mMeshes[i];
//        for (int j = 0; j < mesh->mNumVertices; j++)
//        {
//            const aiVector3D *position = &(mesh->mVertices[j]);
//            vertices.push_back(position->x);
//            vertices.push_back(position->y);
//            vertices.push_back(position->z);
//            vertices.push_back(1.0f);
//        }
//        for (int j = 0; j < mesh->mNumFaces; j++) {
//            const aiFace *face = &(mesh->mFaces[j]);
//            indices.push_back(face->mIndices[0]);
//            indices.push_back(face->mIndices[1]);
//            indices.push_back(face->mIndices[2]);
//        }
//    }
//
//    GLuint VAO = 0;
//    GLuint VBO_vertices = 0;
//    GLuint VBO_indices = 0;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO_vertices);
//    glGenBuffers(1, &VBO_indices);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), nullptr, GL_STATIC_DRAW);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
//
//     Aloca memória e popula o buffer com os indices
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indices);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), nullptr, GL_STATIC_DRAW);
//    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(int), indices.data());
//    
//    GLuint location = 0;
//    GLuint dimensions = 4;
//    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, dimensions * sizeof(float), 0);
//    glEnableVertexAttribArray(location);
//
//     Desligar VAO
//    glBindVertexArray(0);
//
//    return VAO;
//}
//
//GLuint buildTriangle()
//{
//    GLfloat vertices[] = {
//        -0.5f,
//        0.5f,
//        0.0f,
//        1.0f,
//        0.0f,
//        0.75f,
//        0.0f,
//        1.0f,
//        0.5f,
//        0.5f,
//        0.0f,
//        1.0f,
//    };
//
//    GLuint indices[] = {
//        0, 1, 2};
//    GLuint VAO = 0, VBO_vertices = 0, VBO_indices = 0;
//
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO_vertices);
//    glGenBuffers(1, &VBO_indices);
//
//    glBindVertexArray(VAO);
//
//     Aloca memória e popula o buffer com os vértices
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), nullptr, GL_STATIC_DRAW);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//
//     Aloca memória e popula o buffer com os indices
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indices);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), nullptr, GL_STATIC_DRAW);
//    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
//
//    GLuint location = 0;
//    GLuint dimensions = 4;
//    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(location);
//
//     Desligar VAO
//    glBindVertexArray(0);
//
//    return VAO;
//}

int main()
{
    float last_time = 0.0f;
    auto delta_time = std::make_shared<float>(0.0f);

    Window window(900, 500, "Universe");
    ShaderProgram shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    auto camera_pos = glm::vec3(40.0f, 4.0f, 60.0f);
    auto camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    auto camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    CameraFree camera
    {
        camera_pos, 
        camera_target, 
        camera_up
    };
    camera.linkDeltaTime(delta_time);

    Model dragon("Dragon 2.5_dae.dae");

    glfwSetWindowUserPointer(window.getWindow(), &camera);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (window.isOpen())
    {
        window.clear();
        float current_time = glfwGetTime();
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