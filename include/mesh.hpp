#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <memory>
#include <unordered_map>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <texture.hpp>
#include <shader.hpp>

enum Location {
	Position = 0,
	Normal = 1,
	UVTexture = 2,
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Mesh {
public:
	Mesh(const aiMesh* mesh, const aiMaterial* material, const std::string& modelName) {
		std::vector<Vertex> vertices(mesh->mNumVertices);
		this->modelName = modelName;

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

		VAO = createVAO(vertices, indices);
	}

	GLuint createVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
		GLuint VBO = 0, indicesBuffer = 0;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &indicesBuffer);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		setShaderLocations();

		glBindVertexArray(0);

		return VAO;
	}

	void setShaderLocations() {
		glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(Position);

		glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(Normal);

		glVertexAttribPointer(UVTexture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(UVTexture);
	}

	void bindTextures() const {
		std::vector<const char*> textureType = {
			"ambient", "normal", "ao", "roughness", "emissive"
		};

		for (int i = 0; i < textureType.size(); i++) {
			if (textures.contains(textureType[i])) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures.at(textureType[i]).id);
			}
		}
	}

	void unbindTextures() const {
		for (int i = 0; i < TEXTURETYPECOUNT; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void render(const Shader& shader) const {
		glBindVertexArray(VAO);
		bindTextures();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(numTriangles), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		unbindTextures();
	}

	GLuint VAO;
	size_t numTriangles;

	std::string modelName;
	std::unordered_map<std::string, Texture> textures;
};

#endif