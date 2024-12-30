#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <filesystem>
#include <unordered_map>
#include <iostream>

#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>

#include <shader.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <mesh.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class Model {
public:
	Model(std::filesystem::path modelPath) {
		modelName = modelPath.filename().replace_extension("").string();
		if (!std::filesystem::exists(modelPath)) {
			std::cerr << "Error: File not found at " << modelPath << std::endl;
		}
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(modelPath.string(),
			aiProcess_Triangulate |
			aiProcess_FlipUVs
		);

		loadMeshes(scene, scene->mRootNode);
	}

	void loadMeshes(const aiScene* scene, const aiNode* node) {
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			int mesh_index = node->mMeshes[i];
			const aiMesh* assimpMesh = scene->mMeshes[mesh_index];
			aiMaterial* assimpMaterial = scene->mMaterials[assimpMesh->mMaterialIndex];
			auto mesh = Mesh(assimpMesh, assimpMaterial, modelName);
			this->meshes.push_back(mesh);
		}

		// Recursivamente cria as meshes filhas dentro da hierarquia de meshes do assimp
		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			loadMeshes(scene, node->mChildren[i]);
		}
	}

	void addTexture(TextureType textureType, Texture texture, size_t mesh_id) {
		if (textureType == TextureType::AMBIENT) {
			meshes[mesh_id].textures["ambient"] = texture;
			textureFlags |= (1 << TextureType::AMBIENT);
		}
		else if (textureType == TextureType::NORMAL) {
			meshes[mesh_id].textures["normal"] = texture;
			textureFlags |= (1 << TextureType::NORMAL);
		}
		else if (textureType == TextureType::AO) {
			meshes[mesh_id].textures["ao"] = texture;
			textureFlags |= (1 << TextureType::AO);
		}
		else if (textureType == TextureType::ROUGHNESS) {
			meshes[mesh_id].textures["roughness"] = texture;
			textureFlags |= (1 << TextureType::ROUGHNESS);
		}
		else if (textureType == TextureType::EMISSIVE) {
			meshes[mesh_id].textures["emissive"] = texture;
			textureFlags |= (1 << TextureType::EMISSIVE);
		}
	}

	void render(const Shader& shader, CameraFree& camera, glm::vec3 t, glm::vec3 r, float s) {
		glm::mat4 v = camera.getViewMatrix();
		glm::mat4 p = camera.getProjectionMatrix();

		shader.use();
		shader.setMat4("view", v);
		shader.setMat4("projection", p);

		shader.setVec3("translation", t);
		shader.setVec3("rotation", r);
		shader.setFloat("scaling", s);

		shader.setVec3("camera_position", camera.position);

		for (auto& mesh : meshes)
			mesh.render(shader);

		shader.unbind();
	}

	std::string modelName;
	std::vector<Mesh> meshes;
	int textureFlags;

	glm::vec3 position;
	glm::vec3 rotation;
	float scaling;
};

#endif