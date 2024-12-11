#include <terrain.hpp>
#include <stb_image.h>
#include <shader.hpp>
#include <GLM/gtc/noise.hpp>
#include <camera.hpp>

Terrain::Terrain(const float terrainWidth, const float terrainDepth)
{
	const float blockWidth = static_cast<float>(terrainWidth) / numBlocksX;
	const float blockDepth = static_cast<float>(terrainDepth) / numBlocksZ;

	texture = Texture("./assets/terrain/diffuse.jpg");

	for (size_t z = 0; z < numBlocksZ; z++) {
		for (size_t x = 0; x < numBlocksX; x++) {
			blocks.emplace_back(x * blockWidth, z * blockDepth, terrainWidth, terrainDepth);
		}
	}
}

float Terrain::getHeight(glm::vec2 position)
{
	return glm::perlin(position / perlinScalingFactor);
}

void Terrain::render(Shader& shader, CameraFree& camera) const
{
	glm::mat4 v = camera.getViewMatrix();
	glm::mat4 p = camera.getProjectionMatrix();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	shader.use();
	shader.setMat4("view", v);
	shader.setMat4("projection", p);

	shader.setVec3("translation", glm::vec3(0.0f));
	shader.setVec3("rotation", glm::vec3(0.0f));
	shader.setFloat("scaling", 1.0f);
	for (const auto& block : this->blocks)
		if (block.isVisible(camera))
			block.render();

	shader.reset();
}