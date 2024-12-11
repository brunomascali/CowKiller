#include <model.hpp>
#include <GLM/ext/matrix_float4x4.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <iostream>
#include <texture.hpp>
#include <filesystem>
#include <camera.hpp>
#include <GLM/gtx/string_cast.hpp>


Model::Model(std::filesystem::path modelPath)
	:
	textures(std::make_shared<std::vector<Texture>>())
{
	modelName = modelPath.filename().replace_extension("").string();
	if (!std::filesystem::exists(modelPath)) {
		std::cerr << "Error: File not found at " << modelPath << std::endl;
	}
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath.string(),
		aiProcess_Triangulate |
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

void Model::render(const Shader& shader, CameraFree& camera, glm::vec3 translation, glm::vec3 rotation, float scaling)
{
	glm::mat4 v = camera.getViewMatrix();
	glm::mat4 p = camera.getProjectionMatrix();

	shader.use();
	shader.setMat4("view", v);
	shader.setMat4("projection", p);

	shader.setVec3("translation", translation);
	shader.setVec3("rotation", rotation);
	shader.setFloat("scaling", scaling);

	for (auto& mesh : meshes)
		mesh.render();

	shader.reset();
}
