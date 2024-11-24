#include <terrain.hpp>
#include <stb_image.h>
#include <shader.hpp>
#include <GLM/gtc/noise.hpp>
#include <camera.hpp>

Terrain::Terrain(uint32_t terrainWidth, uint32_t terrainDepth)
	: heightmapTexture("./assets/terrain/heightmap.png"),
	normalsTexture("./assets/terrain/normals.jpeg")
{
	int width, depth, numChannels;
	unsigned char* data = stbi_load("./assets/terrain/heightmap.png", &width, &depth, &numChannels, 0);

	height.resize(depth);
	for (size_t i = 0; i < depth; i++) height[i].resize(width);

	for (size_t z = 0; z < depth; z++) {
		for (size_t x = 0; x < width; x++) {
			size_t index = ((static_cast<size_t>(z) * depth) + x) * numChannels;
			height[z][x] = data[index];
		}
	}

	unsigned int numBlocksX = width / terrainWidth;
	unsigned int numBlocksZ = depth / terrainDepth;

	for (size_t z = 1; z < depth / terrainDepth; z++) {
		for (size_t x = 1; x < width / terrainWidth; x++) {
			blocks.emplace_back(x * BLOCK_LENGTH, z * BLOCK_LENGTH, numBlocksX * BLOCK_LENGTH, numBlocksZ * BLOCK_LENGTH);
		}
	}
}

void Terrain::render(Shader& shader, CameraFree& camera) const
{
	glm::mat4 v = camera.getViewMatrix();
	glm::mat4 p = camera.getProjectionMatrix();
	auto idMat4 = glm::mat4(1.0f);
	auto idVec3 = glm::vec3(0.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightmapTexture.id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalsTexture.id);

	shader.use();
	shader.setMat4("model", idMat4);
	shader.setMat4("view", v);
	shader.setMat4("projection", p);

	shader.setInt("heightmap", 0);
	shader.setInt("normalMap", 1);
	for (const auto& block : this->blocks)
		block.render();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
}