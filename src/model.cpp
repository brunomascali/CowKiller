#include "model.hpp"
#include <GLM/ext/matrix_float4x4.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <iostream>

Model::Model(const char *filename)
{
    model = glm::mat4(1.0f);
    rotation = glm::vec3(0.0f);

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename,
                                             aiProcess_CalcTangentSpace |
                                                 aiProcess_Triangulate |
                                                 aiProcess_GenSmoothNormals);

    process_aiNode(scene, scene->mRootNode);
}

void Model::process_aiNode(const aiScene *scene, const aiNode* node)
{
    for (int i = 0; i < node->mNumMeshes; i++) {
        int mesh_index = node->mMeshes[i];
        Mesh mesh(scene->mMeshes[mesh_index]);
        this->meshes.emplace_back(mesh);
    }
    for (int i = 0; i < node->mNumChildren; i++)
    {
        process_aiNode(scene, node->mChildren[i]);
    }
}

void Model::render(ShaderProgram& shader)
{
    shader.setMat4("model", model);
    shader.setVec3("rotation", rotation);
    for (auto &mesh : meshes)
        mesh.render();
}

