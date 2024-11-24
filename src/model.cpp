#include "model.hpp"
#include <GLM/ext/matrix_float4x4.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <iostream>
#include <texture.hpp>
#include <filesystem>
#include <camera.hpp>


Model::Model(std::filesystem::path modelPath)
	:
	model(glm::mat4(1.0f)),
	position(glm::vec3(0.0f)), 
	rotation(glm::vec3(0.0f)),
	textures(std::make_shared<std::vector<Texture>>())
{
	modelName = modelPath.filename().replace_extension("").string();
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath.string(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs);

	createMesh(scene, scene->mRootNode);
}

void Model::createMesh(const aiScene* scene, const aiNode* node)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		int mesh_index = node->mMeshes[i];
		const aiMesh* assimpMesh = scene->mMeshes[mesh_index];
		aiMaterial* assimpMaterial = scene->mMaterials[assimpMesh->mMaterialIndex];
		this->meshes.emplace_back(assimpMesh, assimpMaterial, modelName, this->textures);
	}

	// Recursivamente cria as meshes filhas dentro da hierarquia de meshes do assimp
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		createMesh(scene, node->mChildren[i]);
	}
}

void Model::render(const Shader& shader, CameraFree& camera)
{
	glm::mat4 v = camera.getViewMatrix();
	glm::mat4 p = camera.getProjectionMatrix();

	shader.use();
	shader.setMat4("model", model);
	shader.setMat4("view", v);
	shader.setMat4("projection", p);

	shader.setVec3("rotation", rotation);
	shader.setVec3("translation", position);
	for (auto& mesh : meshes)
		mesh.render();

	shader.reset();
}

