#ifndef MESH_HPP
#define MESH_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <texture.hpp>
#include <vector>
#include <memory>

enum Location {
    Position  = 0,
    Normal    = 1,
    UVTexture = 2
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
public:
    Mesh(const aiMesh* mesh, const aiMaterial* material, const std::string& modelName, std::shared_ptr<std::vector<Texture>> textures);

    GLuint createTexture(const aiTextureType textureType, const aiMaterial* material);

    void render() const;
private:
    GLuint VAO;
    GLuint VBO;
    GLuint indices_buffer;
    size_t numTriangles;

    std::string modelName;
    std::shared_ptr<std::vector<Texture>> textures;
    GLuint textureDiffuseID;
};

#endif