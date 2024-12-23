#ifndef TERRAIN_BLOCK_HPP
#define TERRAIN_BLOCK_HPP

#include <cstdint>
#include <vector>
#include <GLM/vec2.hpp>
#include <glad/glad.h>
#include <mesh.hpp>
#include <camera.hpp>

constexpr int numBlocksX = 8;
constexpr int numBlocksZ = 8;
constexpr float VERTEX_SPACING = 1.0f / 8.0f;
constexpr float PERLINSCALINGFACTOR = 1.0f / 8.0f;

class TerrainBlock {
public:
	// (x, z) estão em coordenadas do terreno global
	TerrainBlock(float posX, float posZ, float worldWidth, float worldDepth) {
		this->blockPosition = glm::vec2(posX, posZ);

		unsigned int blockNumVerticesX = static_cast<unsigned int>(worldWidth / VERTEX_SPACING) + 1;
		unsigned int blockNumVerticesZ = static_cast<unsigned int>(worldDepth / VERTEX_SPACING) + 1;

		// Gera o conjunto de vértices do bloco
		std::vector<Vertex> vertices;
		for (unsigned int z = 0; z < blockNumVerticesZ; z++) {
			for (unsigned int x = 0; x < blockNumVerticesX; x++) {
				Vertex vertex{};
				vertex.position.x = posX + VERTEX_SPACING * static_cast<float>(x);
				vertex.position.z = posZ + VERTEX_SPACING * static_cast<float>(z);
				vertex.position.y = glm::perlin(glm::vec2(vertex.position.x, vertex.position.z) * PERLINSCALINGFACTOR);

				vertex.uv.x = static_cast<float>(vertex.position.x) / worldWidth;
				vertex.uv.y = static_cast<float>(vertex.position.z) / worldDepth;

				//auto v1 = glm::vec3(
				//	vertex.position.x + VERTEX_SPACING,
				//	glm::perlin(glm::vec2(vertex.position.x + VERTEX_SPACING / 4.0f, vertex.position.y) * PERLINSCALINGFACTOR),
				//	vertex.position.y
				//);

				//auto v2 = glm::vec3(
				//	vertex.position.x,
				//	glm::perlin(glm::vec2(vertex.position.x, vertex.position.y + VERTEX_SPACING / 4.0f) * PERLINSCALINGFACTOR),
				//	vertex.position.y + VERTEX_SPACING
				//);

				//glm::vec3 n1 = glm::normalize(glm::cross(v2, v1));
				//vertex.normal = n1;

				vertices.push_back(vertex);
			}
		}

		std::vector<unsigned int> indices;
		for (unsigned int z = 0; z < blockNumVerticesZ - 1; z++) {
			for (unsigned int x = 0; x < blockNumVerticesX - 1; x++) {
				int topLeft = (z * blockNumVerticesZ) + x;
				int bottomLeft = ((z + 1) * (blockNumVerticesZ)) + x;
				int topRight = topLeft + 1;
				int bottomRight = bottomLeft + 1;

				float dx = x != blockNumVerticesX - 1 ? VERTEX_SPACING : -VERTEX_SPACING;
				float dz = z != blockNumVerticesZ - 1 ? VERTEX_SPACING : -VERTEX_SPACING;

				auto v1 = vertices[topRight].position - vertices[topLeft].position;
				auto v2 = vertices[bottomLeft].position - vertices[topLeft].position;

				glm::vec3 n1 = glm::normalize(glm::cross(v2, v1));
				vertices[topLeft].normal = n1;

				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);

				indices.push_back(topLeft);
				indices.push_back(bottomRight);
				indices.push_back(topRight);
			}
		}

		vertexCount = static_cast<GLsizei>(indices.size());

		GLuint VBO = 0, indices_buffer = 0;

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &indices_buffer);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(Position);

		glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(Normal);

		glVertexAttribPointer(UVTexture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(UVTexture);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void render() const {
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	//bool isVisible(CameraFree& camera) const {
	//	return true;
	//}

private:
	GLuint VAO;
	GLsizei vertexCount;
	glm::vec2 blockPosition;
};


#endif