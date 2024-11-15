#include <mesh.hpp>
#include <vector>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>

#include <GLM/vec3.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/mat4x4.hpp>



Mesh::Mesh(const aiMesh* mesh)
{
    glGenVertexArrays(1, &VAO);

    this->add_vertices(mesh);
    this->add_indices(mesh);
    this->add_normals(mesh);
}

void Mesh::add_vertices(const aiMesh* mesh)
{
    glGenBuffers(1, &this->VBO_vertices);
    std::vector<float> vertices;
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        const aiVector3D position = mesh->mVertices[i];
        vertices.push_back(position.x);
        vertices.push_back(position.z);
        vertices.push_back(position.y);

        vertices.push_back(1.0f);
    }
    glBindVertexArray(this->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    GLuint location = 0;
    GLuint dimensions = 4;
    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, dimensions * sizeof(float), 0);
    glEnableVertexAttribArray(location);

    glBindVertexArray(0);
}

void Mesh::add_indices(const aiMesh *mesh)
{
    glGenBuffers(1, &this->VBO_indices);
    std::vector<unsigned int> indices;
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace face = mesh->mFaces[i];
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
    this->numIndices = indices.size();

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->VBO_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());
    glBindVertexArray(0);
}

void Mesh::add_normals(const aiMesh *mesh)
{
    if (!mesh->HasNormals())
        return;
    glGenBuffers(1, &VBO_normals);
    std::vector<float> normals;
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        aiVector3D normal = mesh->mNormals[i];
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, normals.size() * sizeof(float), normals.data());

    GLuint location = 1;
    GLuint dimensions = 3;
    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, dimensions * sizeof(float), 0);
    glEnableVertexAttribArray(location);

    glBindVertexArray(0);
}

void Mesh::render() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
