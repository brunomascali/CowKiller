#include <terrain_block.hpp>
#include <vector>
#include <mesh.hpp>
#include <GLM/gtc/noise.hpp>
#include <terrain.hpp>

TerrainBlock::TerrainBlock(float posX, float posZ, float terrainWidth, float terrainDepth)
{
	this->blockPosition = glm::vec2(posX, posZ);

	unsigned int blockNumVerticesX = static_cast<unsigned int>(terrainWidth / VERTEX_SPACING) + 1;
	unsigned int blockNumVerticesZ = static_cast<unsigned int>(terrainDepth / VERTEX_SPACING) + 1;

	// Gera o conjunto de vértices do bloco
	std::vector<Vertex> vertices;
	for (unsigned int z = 0; z < blockNumVerticesZ; z++) {
		for (unsigned int x = 0; x < blockNumVerticesX; x++) {
			Vertex vertex{};
			vertex.position.x = posX + VERTEX_SPACING * static_cast<float>(x);
			vertex.position.z = posZ + VERTEX_SPACING * static_cast<float>(z);
			//vertex.position.y = glm::perlin(glm::vec2(vertex.position.x, vertex.position.z) * perlinScalingFactor);
			vertex.position.y = 0.0f;

			vertex.uv.x = static_cast<float>(vertex.position.x) / terrainWidth;
			vertex.uv.y = static_cast<float>(vertex.position.z) / terrainDepth;

			auto v1 = glm::vec3(
				vertex.position.x + VERTEX_SPACING,
				glm::perlin(glm::vec2(vertex.position.x + VERTEX_SPACING / 4.0f, vertex.position.y) * perlinScalingFactor),
				vertex.position.y
			);

			auto v2 = glm::vec3(
				vertex.position.x,
				glm::perlin(glm::vec2(vertex.position.x, vertex.position.y + VERTEX_SPACING / 4.0f) * perlinScalingFactor),
				vertex.position.y + VERTEX_SPACING
			);

			glm::vec3 n1 = glm::normalize(glm::cross(v2, v1));
			vertex.normal = n1;

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

	glVertexAttribPointer(Location::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(float) * 0));
	glEnableVertexAttribArray(Location::Position);

	glVertexAttribPointer(Location::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(float) * 3));
	glEnableVertexAttribArray(Location::Normal);

	glVertexAttribPointer(Location::UVTexture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(Location::UVTexture);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void TerrainBlock::render() const
{
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

bool TerrainBlock::isVisible(CameraFree &camera) const
{
	return true;
}
