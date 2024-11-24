#include <terrain_block.hpp>
#include <vector>
#include <mesh.hpp>

TerrainBlock::TerrainBlock(float posX, float posZ, float worldWidth, float worldDepth)
{
	this->blockPosition = glm::vec2(posX, posZ);

	// Gera o conjunto de vértices do bloco
	std::vector<Vertex> vertices;
	for (int z = 0; z < VertexBlockCount; z++) {
		for (int x = 0; x < VertexBlockCount; x++) {
			Vertex vertex{};
			vertex.position.x = posX + vertexSpacing * static_cast<float>(x);
			vertex.position.z = posZ + vertexSpacing * static_cast<float>(z);

			vertex.uv.x = static_cast<float>(vertex.position.x) / worldWidth;
			vertex.uv.y = static_cast<float>(vertex.position.z) / worldDepth;

			vertices.push_back(vertex);
		}
	}

	// Indices dos triângulos do terreno
	std::vector<unsigned int> indices;
	for (int z = 0; z < VertexBlockCount - 1; ++z) {
		for (int x = 0; x < VertexBlockCount - 1; ++x) {
			int topLeft = z * VertexBlockCount + x;
			int bottomLeft = (z + 1) * VertexBlockCount + x;
			int topRight = z * VertexBlockCount + (x + 1);
			int bottomRight = (z + 1) * VertexBlockCount + (x + 1);

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);

			indices.push_back(topLeft);
			indices.push_back(bottomRight);
			indices.push_back(topRight);
		}
	}
	this->vertexCount = indices.size();

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