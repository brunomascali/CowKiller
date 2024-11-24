#include <mesh.hpp>
#include <vector>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GLM/vec3.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/mat4x4.hpp>
#include <unordered_map>
#include <memory>
#include <texture.hpp>

std::unordered_map<std::string, GLuint> textures;

Mesh::Mesh(const aiMesh* mesh, const aiMaterial* material, const std::string& modelName, std::shared_ptr<std::vector<Texture>> textures)
	: modelName(modelName), textures(textures)
{
	std::vector<Vertex> vertices(mesh->mNumVertices);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D p = mesh->mVertices[i];
		vertices[i].position = glm::vec3(p.x, p.y, p.z);

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
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	this->numTriangles = indices.size();

	textureDiffuseID = createTexture(aiTextureType_DIFFUSE, material);

	GLuint VBO = 0, indicesBuffer = 0;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &indicesBuffer);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(Location::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(float) * 0));
	glEnableVertexAttribArray(Location::Position);

	glVertexAttribPointer(Location::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(float) * 3));
	glEnableVertexAttribArray(Location::Normal);

	glVertexAttribPointer(Location::UVTexture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(Location::UVTexture);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

GLuint Mesh::createTexture(const aiTextureType textureType, const aiMaterial* material)
{
	aiString textureFile;
	if (material->GetTexture(textureType, 0, &textureFile) == AI_SUCCESS)
	{
		const std::string textureFilename(textureFile.C_Str());
		auto texture_ptr = find_if(textures->begin(), textures->end(), [&](const Texture& texture) {
			return texture.path == textureFilename;
			});

		// Criar a textura caso ela não exista ainda
		if (texture_ptr == textures->end())
		{
			Texture texture(modelName, textureFilename);
			textures->emplace_back(texture);
			return texture.id;
		}
	}
	return 0;
}

void Mesh::render() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDiffuseID);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(numTriangles), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
