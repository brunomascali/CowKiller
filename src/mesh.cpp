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
    std::vector<Vertex> vertices(mesh->mNumVertices);

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        const aiVector3D p = mesh->mVertices[i];
        vertices[i].position = glm::vec4(p.x, p.y, p.z, 1.0f);

        if (mesh->HasNormals())
        {
            const aiVector3D n = mesh->mNormals[i];
            vertices[i].normal = glm::vec3(n.x, n.y, n.z);
        }

        if (mesh->HasTextureCoords(0))
        {
            const aiVector3D uv = mesh->mTextureCoords[0][i];
            vertices[i].uv = glm::vec2(uv.x, uv.y);
        }
        else {
            vertices[i].uv = glm::vec2(0.0f);
        }
    }

    std::vector<unsigned int> indices;
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace face = mesh->mFaces[i];
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
    this->numTriangles = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &indices_buffer);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(Location::Position, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(float) * 0));
    glEnableVertexAttribArray(Location::Position);

    glVertexAttribPointer(Location::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location::Normal);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::render() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, this->numTriangles, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
