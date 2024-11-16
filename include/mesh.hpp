#ifndef MESH_HPP
#define MESH_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

enum Location {
    Position  = 0,
    Normal    = 1,
    UVTexture = 2
};

struct Vertex {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
public:
    Mesh(const aiMesh* mesh);

    void render() const;

private:
    GLuint VAO;
    GLuint VBO;
    GLuint indices_buffer;
    unsigned int numTriangles;
};

#endif